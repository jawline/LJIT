#ifndef _JSTATEMENT_DEF_H_
#define _JSTATEMENT_DEF_H_
#include <memory>
#include <vector>
#include "../utils/bytebuffer.h"

namespace JIT {

    enum StatementType {
        Atom,
        Add,
        Subtract,
        Multiply,
        Divide,
        Get,
        Set,
        Print
    };

    typedef std::shared_ptr<class Statement> SafeStatement;

    class Statement {
        private:
          StatementType _type;
          int64_t _val;
          std::vector<SafeStatement> args;
        public:
          Statement(int64_t val);
          Statement(StatementType type, std::vector<SafeStatement> const& args);
          void write(Assembler::ByteBuffer& buffer);
    };
}

#endif //_JSTATEMENT_DEF_H_
