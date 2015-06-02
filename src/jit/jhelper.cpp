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

size_t Helper::jumpRelative(Assembler::ByteBuffer& buffer, int32_t distance) {
    //jmp $+distance
    buffer.insert((uint8_t)0xE9);
    size_t addr = buffer.current();
    buffer.insert(distance);
    return addr;
}

size_t Helper::jumpRelativeTopEqualZero(Assembler::ByteBuffer& buffer, int32_t distance) {

    //pop RAX
    buffer.insert((uint8_t)0x58);
    
    //cmp rax, 0
    uint8_t cmpRax0[] = { 0x48, 0x83, 0xF8, 0x00 };
    buffer.insert(cmpRax0, sizeof(cmpRax0));
    
    //je $+distance
    buffer.insert((uint8_t)0x0F);
    buffer.insert((uint8_t)0x84);
    size_t addr = buffer.current();
    buffer.insert(distance);
    return addr;
}

void Helper::functionEntryPushArgs(unsigned int num, Assembler::ByteBuffer& buffer) {
    
    //The first arg is moved into r12
    if (num < 2) {
        return;
    }

    for (unsigned int i = 0; i < num - 1; i++) {
        switch (i) {
            case 0:
                //push rsi
                buffer.insert((uint8_t) 0x56);
                break;
            case 1:
                //push rdx
                buffer.insert((uint8_t) 0x52);
                break;
            case 2:
                //push rcx
                buffer.insert((uint8_t) 0x51);
                break;
            case 3:
                //push r8
                buffer.insert((uint8_t) 0x41);
                buffer.insert((uint8_t) 0x50);
                break;
            case 4:
                //push r9
                buffer.insert((uint8_t) 0x41);
                buffer.insert((uint8_t) 0x51);
            default:
                printf("Can't handle this many args\n");
                return;
        }
    }
}

void Helper::functionExitDiscardArgs(unsigned int num, Assembler::ByteBuffer& buffer) {

    //First argument is put in r12, discard others
    if (num < 2) {
        return;
    }

    //TODO: as an optimization just add sizeof(int64_t) * num-1
    for (unsigned int i = 0; i < num - 1; i++) {
        uint8_t addRsp8[] = { 0x48, 0x83, 0xC4, (uint8_t) sizeof(int64_t) };
        buffer.insert(addRsp8, sizeof(addRsp8));
    }
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
    auto start_addr = ((uint8_t*)ptr) + start;
    *((int64_t*)start_addr) = (int64_t) newAddress;
}

void Helper::pushArgumentTop(int argN, Assembler::ByteBuffer& buffer) {
    if (argN == 0) {
        buffer.insert((uint8_t)0x41);
        buffer.insert((uint8_t)0x54);
    } else {
        //push [ebp - (argN-1)*8]
        uint8_t pushArg[] = { 0xFF, 0x75, (uint8_t)((-argN - 1) * 8) };
        buffer.insert(pushArg, sizeof(pushArg));
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
            printf("Outside of arg register range (%i)\n", num);
            return;
    }
}

void Helper::popResult(Assembler::ByteBuffer& buffer) {
    buffer.insert((uint8_t)0x58);
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
