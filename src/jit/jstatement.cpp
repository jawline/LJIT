#include "jstatement.h"
#include "jhelper.h"

using namespace JIT;

Statement::Statement(int64_t val) {
  _type = Atom;
  _val = val;
}

Statement::Statement(StatementType type, SafeStatement lhs) {
  _type = type;
  _lhs = lhs;
}

Statement::Statement(StatementType type, SafeStatement lhs, SafeStatement rhs) {
  _type = type;
  _lhs = lhs;
  _rhs = rhs;
}

int64_t ScopeSetFn(int* scope, int64_t id, int64_t val) {
  printf("If I knew how I would set %li to %li on the current scope\n", id, val);
  return val;
}

int64_t ScopeGetFn(int* scope, int64_t id) {
  printf("If I knew how I would get %li from the current scope\n", id);
  return 0;
}

int64_t PrintFn(int* scope, int64_t val) {
  printf("Print Call: %li\n", val);
  return val;
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
      Helper::callFunction((void*) ScopeSetFn, buffer);
      break;
    case Get:
      _lhs->write(buffer);
      Helper::setArgumentZeroScope(buffer);
      Helper::setArgumentStackTop(1, buffer);
      Helper::callFunction((void*) ScopeGetFn, buffer);
      break;
    case Print:
      _lhs->write(buffer);
      Helper::setArgumentZeroScope(buffer);
      Helper::setArgumentStackTop(1, buffer);
      Helper::callFunction((void*) PrintFn, buffer);
      break;
    default:
      printf("Could not JIT\n");
      break;
  }
}
