#ifndef _JIT_HELPER_DEF_H_
#define _JIT_HELPER_DEF_H_
#include <stdint>
#include "../utils/bytebuffer.h"

namespace JIT {
  class Helper {
  public:
    static void insertPrologue(ByteBuffer& buffer);
    static void insertEpilogue(ByteBuffer& buffer);
    static void pushNumber(int64_t value, ByteBuffer& buffer);
    static void addTopTwoStack(ByteBuffer& buffer);
  };
}

#endif //_JIT_HELPER_DEF_H_