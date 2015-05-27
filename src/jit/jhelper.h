#ifndef _JIT_HELPER_DEF_H_
#define _JIT_HELPER_DEF_H_
#include <stdint>
#include "../utils/bytebuffer.h"

namespace JIT {
  
  typedef int64_t (*JFPTR)();
  
  class Helper {
  public:
    static void insertPrologue(ByteBuffer& buffer);
    static void insertEpilogue(ByteBuffer& buffer);
    static void pushNumber(int64_t value, ByteBuffer& buffer);
    static void addTopTwoStack(ByteBuffer& buffer);
    static JFPTR prepareFunctionPointer(ByteBuffer const& buffer);
    static void freeFunctionPointer(JFPTR ptr, size_t size);
  };
}

#endif //_JIT_HELPER_DEF_H_
