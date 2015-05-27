#ifndef _JSTATEMENT_DEF_H_
#define _JSTATEMENT_DEF_H_
#include <memory>
#include "../utils/bytebuffer.h"

namespace JIT {

    enum StatementType {
        Atom,
        Add,
        Subtract,
        Multiply,
        Divide
    }

    class Statement {
        private:
          StatementType _type;
          int64_t _val;
          SafeStatement _lhs, _rhs;
        public:
          Statement(int64_t val);
          Statement(StatementType type, SafeStatement lhs, SafeStatement rhs);
          void write(ByteBuffer& buffer);
    };
    
    typedef std::shared_ptr<Statement> SafeStatement;
}

#endif //_JSTATEMENT_DEF_H_
