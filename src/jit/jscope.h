#ifndef _JIT_SCOPE_DEF_H_
#define _JIT_SCOPE_DEF_H_
#include <stdint.h>

namespace JIT {
  class Scope {
  private:
    int64_t* _data;

  public:
    Scope();
    ~Scope();
    
    void set(int64_t idx, int64_t val);
    int64_t get(int64_t idx) const;
  };
}

#endif
