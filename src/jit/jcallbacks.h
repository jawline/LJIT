#ifndef _JIT_CALLBACKS_DEF_H_
#define _JIT_CALLBACKS_DEF_H_
#include <stdint.h>
#include "jscope.h"

namespace JIT {
  class Callbacks {
    public:
      static int64_t print(Scope* scope, int64_t val);
      static int64_t set(Scope* scope, int64_t idx, int64_t val);
      static int64_t get(Scope* scope, int64_t idx);
  };
}

#endif //_JIT_CALLBACKS_DEF_H_
