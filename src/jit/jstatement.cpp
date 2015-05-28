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
      _lhs->write(buffer);
      _rhs->write(buffer);
      Helper::addTopTwoStack(buffer);
      break;
    case Subtract:
      _lhs->write(buffer);
      _rhs->write(buffer);
      Helper::subTopTwoStack(buffer);
      break;
    case Multiply:
      _lhs->write(buffer);
      _rhs->write(buffer);
      Helper::mulTopTwoStack(buffer);
      break;
    case Divide:
      _lhs->write(buffer);
      _rhs->write(buffer);
      Helper::divTopTwoStack(buffer);
      break;
    case Set:
      _lhs->write(buffer);
      _rhs->write(buffer);
      Helper::setArgumentZeroScope(buffer);
      Helper::setArgumentStackTop(2, buffer);
      Helper::setArgumentStackTop(1, buffer);
      Helper::callFunction((void*) Callbacks::set, buffer);
      break;
    case Get:
      _lhs->write(buffer);
      Helper::setArgumentZeroScope(buffer);
      Helper::setArgumentStackTop(1, buffer);
      Helper::callFunction((void*) Callbacks::get, buffer);
      break;
    case Print:
      _lhs->write(buffer);
      Helper::setArgumentZeroScope(buffer);
      Helper::setArgumentStackTop(1, buffer);
      Helper::callFunction((void*) Callbacks::print, buffer);
      break;
    default:
      printf("Could not JIT\n");
      break;
  }
}
