#include "jstatement.h"
#include "jhelper.h"

using namespace JIT;

Statement::Statement(int64_t val) {
  _val = val;
}

Statement::Statement(SafeStatement lhs, SafeStatement rhs) {
  _lhs = lhs;
  _rhs = rhs;
  _add = true;
}

void Statement::write(ByteBuffer& buffer) {
  if (_add) {
    Helper::addTopTwoStack(buffer);
  } else {
    Helper::pushNumber(_val);
  }
}
