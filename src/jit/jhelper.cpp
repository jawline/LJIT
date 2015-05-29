#include "jhelper.h"
#include "jtemplate.h"
#include <string.h>
#include <sys/mman.h>
#include <stdio.h>

using namespace JIT;
using namespace Assembler;

void Helper::pushBasicResult(ByteBuffer& buffer) {
    //push RAX
    buffer.insert((uint8_t)0x50);
}

void Helper::popTwo(ByteBuffer& buffer) {
    //pop RCX
    buffer.insert((uint8_t)0x59);
    //pop RAX
    buffer.insert((uint8_t)0x58);
}

void Helper::insertPrologue(ByteBuffer& buffer) {
    buffer.insert(Template::prologue, Template::prologueSize());
}

void Helper::insertEpilogue(ByteBuffer& buffer) {
    buffer.insert(Template::epilogue, Template::epilogueSize());
}

size_t Helper::loadAddress(void* address, ByteBuffer& buffer) {
    //mov _val, RAX
    uint8_t mrax[] = { 0x48, 0xB8 };
    buffer.insert(mrax, sizeof(mrax));
    size_t addr = buffer.current();
    buffer.insert((int64_t) address);
    return addr;
}

size_t Helper::callFunction(void* fnPtr, ByteBuffer& buffer) {
    //TODO: Work out the damn call addr instruction and use that instead of loading it into a register.
    size_t addr = loadAddress(fnPtr, buffer);
    //call fnPtr
    buffer.insert((uint8_t)0xFF);
    buffer.insert((uint8_t)0xD0);
    pushBasicResult(buffer);
    return addr;
}

void Helper::pushNumber(int64_t value, ByteBuffer& buffer) {
    //mov _val, RAX
    uint8_t mrax[] = { 0x48, 0xB8 };
    buffer.insert(mrax, sizeof(mrax));
    buffer.insert((int64_t) value);

    pushBasicResult(buffer);
}

void Helper::addTopTwoStack(ByteBuffer& buffer) {
    popTwo(buffer);
    
    //add RCX, RAX
    uint8_t addRcxRax[] = {0x48, 0x01, 0xC8};
    buffer.insert(addRcxRax, sizeof(addRcxRax));
    
    pushBasicResult(buffer);
}

void Helper::subTopTwoStack(ByteBuffer& buffer) {
    popTwo(buffer);
    
    //sub RCX, RAX
    uint8_t subRcxRax[] = {0x48, 0x29, 0xC8};
    buffer.insert(subRcxRax, sizeof(subRcxRax));
    
    pushBasicResult(buffer);   
}

void Helper::mulTopTwoStack(ByteBuffer& buffer) {
    popTwo(buffer);
    
    //mul rcx
    uint8_t mulRcx[] = {0x48, 0xF7, 0xE1};
    buffer.insert(mulRcx, sizeof(mulRcx));
    
    pushBasicResult(buffer);   
}

void Helper::divTopTwoStack(ByteBuffer& buffer) {
    popTwo(buffer);
    
    //div rcx
    uint8_t divRcx[] = {0x48, 0xF7, 0xF1};
    buffer.insert(divRcx, sizeof(divRcx));
    
    pushBasicResult(buffer);   
}

void Helper::setArgument(unsigned int num, int64_t val, Assembler::ByteBuffer& buffer) {
    switch (num) {
        case 0: {
            uint8_t movRdi[] = { 0x48, 0xBF };
            buffer.insert(movRdi, sizeof(movRdi));
            buffer.insert(val);
            break;
        }
        case 1: {
            uint8_t movRsi[] = { 0x48, 0xBE };
            buffer.insert(movRsi, sizeof(movRsi));
            buffer.insert(val);
            break;            
        }
        case 2: {
            uint8_t movRdx[] = { 0x48, 0xBA };
            buffer.insert(movRdx, sizeof(movRdx));
            buffer.insert(val);
            break;            
        }
        default:
            printf("UNSUPPORTED ARG NUM %i\n", num);
            return;
    }
}

void Helper::updateAddress(JFPTR ptr, size_t start, void* newAddress) {
    *(((void**)ptr) + start) = newAddress;
}

void Helper::setArgumentZeroScope(Assembler::ByteBuffer& buffer) {
    uint8_t mvR12Rdi[] = { 0x4C, 0x89, 0xE7 };
    buffer.insert(mvR12Rdi, sizeof(mvR12Rdi));
}

void Helper::setArgumentStackTop(unsigned int num, Assembler::ByteBuffer& buffer) {
    switch (num) {
        case 0: {
            //pop rdi
            buffer.insert((uint8_t)0x5F);
            break;
        }
        case 1: {
            //pop rsi
            buffer.insert((uint8_t)0x5E);
            break;            
        }
        case 2: {
            //pop rdx
            buffer.insert((uint8_t)0x5A);
            break;            
        }
        case 3: {
            //pop rcx
            buffer.insert((uint8_t)0x59);
            break;            
        }
        case 4: {
            //pop r8
            buffer.insert((uint8_t)0x41);
            buffer.insert((uint8_t)0x58);
            break;            
        }
        case 5: {
            //pop r9
            buffer.insert((uint8_t)0x41);
            buffer.insert((uint8_t)0x59);
            break;            
        }
        default:
            printf("UNSUPPORTED ARG NUM %i\n", num);
            return;
    }
}

JFPTR Helper::prepareFunctionPointer(ByteBuffer const& buffer) {
  //mmap some executable and writable memory and copy machine code into it
  void* mem = mmap(nullptr, buffer.current(), PROT_WRITE | PROT_EXEC, MAP_ANON | MAP_PRIVATE, -1, 0);
  memcpy(mem, buffer.raw(), buffer.current());
  return (JFPTR) mem;
}

void Helper::freeFunctionPointer(JFPTR ptr, size_t size) {
  munmap((void*)ptr, size);
}
