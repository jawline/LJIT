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

void Do() {
  printf("I DO\n");
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
      printf("SET\n");
      Helper::callFunction((void*) Do, buffer);
      break;
    default:
      printf("Could not JIT\n");
      break;
  }
}
