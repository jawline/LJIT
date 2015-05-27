#ifndef _JIT_HELPER_DEF_H_
#define _JIT_HELPER_DEF_H_
#include <stdint.h>
#include "../utils/bytebuffer.h"

namespace JIT {
  
  typedef int64_t (*JFPTR)();
  
  class Helper {
  private:
    static void pushBasicResult(Assembler::ByteBuffer& buffer);
    static void popTwo(Assembler::ByteBuffer& buffer);
    static void loadAddress(void* address, Assembler::ByteBuffer& buffer);

  public:
    static void insertPrologue(Assembler::ByteBuffer& buffer);
    static void insertEpilogue(Assembler::ByteBuffer& buffer);
    static void pushNumber(int64_t value, Assembler::ByteBuffer& buffer);
    static void callFunction(void* fnPtr, Assembler::ByteBuffer& buffer);
    static void addTopTwoStack(Assembler::ByteBuffer& buffer);
    static void subTopTwoStack(Assembler::ByteBuffer& buffer);
    static void mulTopTwoStack(Assembler::ByteBuffer& buffer);
    static void divTopTwoStack(Assembler::ByteBuffer& buffer);
    static JFPTR prepareFunctionPointer(Assembler::ByteBuffer const& buffer);
    static void freeFunctionPointer(JFPTR ptr, size_t size);
  };
}

#endif //_JIT_HELPER_DEF_H_
