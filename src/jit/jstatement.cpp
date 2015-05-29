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
  _args = args;
}

Statement::Statement(StatementType type, void* callback, std::vector<SafeStatement> const& args) {
  _type = type;
  _callback = callback;
  _args = args;
}

void Statement::updateCallback(void* callback) {
  _callback = callback;
}

void* Statement::getCallback() const {
  return _callback;
}

void Statement::write(Assembler::ByteBuffer& buffer, std::vector<std::pair<Statement*, size_t>>& unresolvedList) {
  switch (_type) {
    case Atom:
      Helper::pushNumber(_val, buffer);
      break;
    case Add:
      _args[0]->write(buffer);
      _args[1]->write(buffer);
      Helper::addTopTwoStack(buffer);
      break;
    case Subtract:
      _args[0]->write(buffer);
      _args[1]->write(buffer);
      Helper::subTopTwoStack(buffer);
      break;
    case Multiply:
      _args[0]->write(buffer);
      _args[1]->write(buffer);
      Helper::mulTopTwoStack(buffer);
      break;
    case Divide:
      _args[0]->write(buffer);
      _args[1]->write(buffer);
      Helper::divTopTwoStack(buffer);
      break;
    case NativeCallback:
      for (unsigned int i = 0; i < _args.size(); i++) {
        _args[i]->write(buffer);
      }
      Helper::setArgumentZeroScope(buffer);
      for (int i = _args.size() - 1; i >= 0; i--) {
        Helper::setArgumentStackTop(i+1, buffer);
      }
      size_t addressStart = Helper::callFunction(_callback, buffer);
      if (_callback == nullptr) {
        printf("Cannot produce nativecall, _callback unresolved\n");
        printf("Adding to unresolved list\n");
        unresolvedList.push_back(std::pair<Statement*, size_t location>(this, addressStart))
      }
      break;
    default:
      printf("Could not JIT\n");
      break;
  }
}
