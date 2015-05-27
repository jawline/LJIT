#include "jstatement.h"
#include "jhelper.h"

using namespace JIT;

Statement::Statement(int64_t val) {
  _val = val;
}

void Statement::write(ByteBuffer& buffer) {
  Helper::pushNumber(_val);
}
