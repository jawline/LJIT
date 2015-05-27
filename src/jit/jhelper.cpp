#include "jhelper.h"
#include "jtemplate.h"
#include <string.h>
#include <sys/mman.h>

using namespace JIT;

void Helper::pushBasicResult(ByteBuffer& buffer) {
    //Convention, basic (atom, add, sub, mul, div) results go to RAX on intel64
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
    buffer.insert(Template::prologue, sizeof(Template::prologue));
}

void Helper::insertEpilogue(ByteBuffer& buffer) {
    buffer.insert(Template::epilogue, sizeof(Template::epilogue));
}

void Helper::pushNumber(int64_t value, ByteBuffer& buffer) {
    //mov _val, RAX
    uint8_t mrax[2] = { 0x48, 0xB8 };
    buffer.insert(mrax, 2);
    buffer.insert((int64_t) _val);

    //push rax
    pushBasicResult(buffer);
}

void Helper::addTopTwoStack(ByteBuffer& buffer) {
    //pop rcx; pop rax
    popTwo(buffer);
    //add RCX, RAX
    buffer.insert((uint8_t*){0x48, 0x01, 0xC8}, 3);
    //push rax
    pushBasicResult(buffer);
}

void Helper::subTopTwoStack(ByteBuffer& buffer) {
    //pop rcx; pop rax
    popTwo(buffer);
    //sub RCX, RAX
    buffer.insert((uint8_t*){0x48, 0x29, 0xC8}, 3);
    //push rax
    pushBasicResult(buffer);   
}

JFPTR Helper::prepareFunctionPointer(ByteBuffer const& buffer) {
  //mmap some executable and writable memory and copy machine code into it
  void* mem = mmap(nullptr, buffer.current(), PROT_WRITE | PROT_EXEC, MAP_ANON | MAP_PRIVATE, -1, 0);
  memcpy(mem, buffer.raw(), buffer.current());
  return (JFPTR) mem;
}

void Helper::freeFunctionPointer(JFPTR ptr, size_t size) {
  munmap(mem, size);
}
