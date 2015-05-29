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
        If,
        NativeCallback
    };

    typedef std::shared_ptr<class Statement> SafeStatement;

    class Statement {
        private:
          StatementType _type;
          int64_t _val;
          void* _callback;
          std::vector<SafeStatement> _args;
        public:
          Statement(int64_t val);
          Statement(StatementType type, std::vector<SafeStatement> const& args);
          Statement(StatementType type, void* callback, std::vector<SafeStatement> const& args);
          void write(Assembler::ByteBuffer& buffer, std::vector<std::pair<Statement*, size_t>>& unresolvedList);
          void* getCallback() const;
          int getNumArgs() const;
          void updateCallback(void* newCallback);
    };
}

#endif //_JSTATEMENT_DEF_H_
