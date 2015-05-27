#ifndef _JSTATEMENT_DEF_H_
#define _JSTATEMENT_DEF_H_
#include "../utils/bytebuffer.h"

namespace JIT {
  class Statement {
  private:
    int64_t val;
    bool add;
  public:
    Statement(int64_t val);
    void write(ByteBuffer& buffer);
  };
}

#endif //_JSTATEMENT_DEF_H_
