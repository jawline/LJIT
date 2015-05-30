#ifndef _JIT_CALLBACKS_DEF_H_
#define _JIT_CALLBACKS_DEF_H_
#include <stdint.h>

namespace JIT {
  class Callbacks {
    public:
      static int64_t print(int64_t val);
      static int64_t unresolved();
      static int64_t set(int64_t idx, int64_t val);
      static int64_t get(int64_t idx);
  };
}

#endif //_JIT_CALLBACKS_DEF_H_
