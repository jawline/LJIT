#ifndef _JIT_HELPER_DEF_H_
#define _JIT_HELPER_DEF_H_
#include <stdint.h>
#include <vector>
#include <string>
#include "../utils/bytebuffer.h"

namespace JIT {
  
  typedef int64_t (*JFPTR)();
  
  class Helper {
  private:
    static void pushBasicResult(Assembler::ByteBuffer& buffer);
    static void popTwo(Assembler::ByteBuffer& buffer);
    static size_t loadAddress(void* address, Assembler::ByteBuffer& buffer);

  public:
    static void insertPrologue(Assembler::ByteBuffer& buffer);
    static void insertEpilogue(Assembler::ByteBuffer& buffer);
    static void pushNumber(int64_t value, Assembler::ByteBuffer& buffer);
    static void popResult(Assembler::ByteBuffer& buffer);
    static size_t callFunction(void* fnPtr, Assembler::ByteBuffer& buffer);
    static void functionEntryPushArgs(unsigned int num, Assembler::ByteBuffer& buffer);
    static void functionExitDiscardArgs(unsigned int num, Assembler::ByteBuffer& buffer);
    static void setArgument(unsigned int num, int64_t val, Assembler::ByteBuffer& buffer);
    static void setArgumentStackTop(unsigned int num, Assembler::ByteBuffer& buffer);
    static void addTopTwoStack(Assembler::ByteBuffer& buffer);
    static void subTopTwoStack(Assembler::ByteBuffer& buffer);
    static void mulTopTwoStack(Assembler::ByteBuffer& buffer);
    static void divTopTwoStack(Assembler::ByteBuffer& buffer);
    static void pushArgumentTop(int argN, Assembler::ByteBuffer& buffer);
    static size_t jumpRelativeTopEqualZero(Assembler::ByteBuffer& buffer, int32_t distance);
    static size_t jumpRelative(Assembler::ByteBuffer& buffer, int32_t distance);
    static void updateAddress(JFPTR ptr, size_t start, void* newAddress);
    static JFPTR prepareFunctionPointer(Assembler::ByteBuffer const& buffer);
    static void freeFunctionPointer(JFPTR ptr, size_t size);
  };
}

#endif //_JIT_HELPER_DEF_H_
