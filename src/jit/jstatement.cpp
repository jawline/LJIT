#include "jstatement.h"
#include "jhelper.h"

using namespace JIT;

Statement::Statement(int64_t val) {
  _type = Atom;
  _val = val;
}

Statement::Statement(StatementType type, SafeStatement lhs, SafeStatement rhs) {
  _type = type;
  _lhs = lhs;
  _rhs = rhs;
}

void Statement::write(ByteBuffer& buffer) {
  switch (_type) {
    case Atom:
      Helper::addTopTwoStack(buffer);
      break;
    case Add:
      Helper::addTopTwoStack(buffer);
      break;
    default:
      printf("Could not JIT\n");
      break;
  }
}
