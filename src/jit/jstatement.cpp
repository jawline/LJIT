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
      _args[0]->write(buffer, unresolvedList);
      _args[1]->write(buffer, unresolvedList);
      Helper::addTopTwoStack(buffer);
      break;
    case Subtract:
      _args[0]->write(buffer, unresolvedList);
      _args[1]->write(buffer, unresolvedList);
      Helper::subTopTwoStack(buffer);
      break;
    case Multiply:
      _args[0]->write(buffer, unresolvedList);
      _args[1]->write(buffer, unresolvedList);
      Helper::mulTopTwoStack(buffer);
      break;
    case Divide:
      _args[0]->write(buffer, unresolvedList);
      _args[1]->write(buffer, unresolvedList);
      Helper::divTopTwoStack(buffer);
      break;
    case NativeCallback: {
      Helper::setArgumentZeroScope(buffer);
      for (int i = _args.size(); i > 0; i--) {
        _args[_args.size() - i]->write(buffer, unresolvedList);
        Helper::setArgumentStackTop(_args.size() - i + 1, buffer);
      }
      size_t addressStart = Helper::callFunction(_callback ? _callback : ((void*)Callbacks::unresolved), buffer);
      if (_callback == nullptr) {
        printf("Cannot produce nativecall, _callback unresolved\n");
        printf("Adding to unresolved list\n");
        unresolvedList.push_back(std::pair<Statement*, size_t>(this, addressStart));
      }
      break;
    }
    default:
      printf("Could not JIT\n");
      break;
  }
}
