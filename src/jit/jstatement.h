#ifndef _JSTATEMENT_DEF_H_
#define _JSTATEMENT_DEF_H_
#include <memory>
#include "../utils/bytebuffer.h"

namespace JIT {
    
    class Statement {
        private:
          int64_t _val;
          bool _add;
          SafeStatement _lhs, _rhs;
        public:
          Statement(int64_t val);
          Statement(SafeStatement lhs, SafeStatement rhs);
          void write(ByteBuffer& buffer);
    };
    
    typedef std::shared_ptr<Statement> SafeStatement;
}

#endif //_JSTATEMENT_DEF_H_
