#include "jstatement.h"
#include "jhelper.h"
#include "jcallbacks.h"

using namespace JIT;

Statement::Statement(int64_t val) {
  _type = Atom;
  _val = val;
}

Statement::Statement(StatementType type, std::vector<SafeStatement> const& args) {
  _type = type;
  _lhs = lhs;
}

void Statement::write(Assembler::ByteBuffer& buffer) {
  switch (_type) {
    case Atom:
      Helper::pushNumber(_val, buffer);
      break;
    case Add:
      args[0]->write(buffer);
      args[1]->write(buffer);
      Helper::addTopTwoStack(buffer);
      break;
    case Subtract:
      args[0]->write(buffer);
      args[1]->write(buffer);
      Helper::subTopTwoStack(buffer);
      break;
    case Multiply:
      args[0]->write(buffer);
      args[1]->write(buffer);
      Helper::mulTopTwoStack(buffer);
      break;
    case Divide:
      args[0]->write(buffer);
      args[1]->write(buffer);
      Helper::divTopTwoStack(buffer);
      break;
    case NativeCallback:
      for (unsigned int i = 0; i < _args.size(); i++) {
        args[i]->write(buffer);
      }
      Helper::setArgumentZeroScope(buffer);
      for (unsigned int i = _args.size() - 1; i >= 0; i--) {
        Helper::setArgumentStackTop(i+1, buffer);
      }
      Helper::callFunction(_callback, buffer);
      break;
    default:
      printf("Could not JIT\n");
      break;
  }
}
