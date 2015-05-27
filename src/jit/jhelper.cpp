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

void Helper::loadAddress(void* address, ByteBuffer& buffer) {
    //mov _val, RAX
    uint8_t mrax[] = { 0x48, 0xB8 };
    buffer.insert(mrax, sizeof(mrax));
    buffer.insert((int64_t) address);
}

void Helper::callFunction(void* fnPtr, ByteBuffer& buffer) {
    loadAddress(fnPtr, buffer);

    //call rax
    uint8_t crax[] = {0xFF, 0xD0};
    buffer.insert(crax, sizeof(crax));

    pushBasicResult(buffer);
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
            uint8_t movRdi[] = { 0x48, 0xBE };
            buffer.insert(movRdi, sizeof(movRdi));
            buffer.insert(val);
            break;            
        }
        default:
            printf("UNSUPPORTED ARG NUM %i\n", num);
            return;
    }
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
