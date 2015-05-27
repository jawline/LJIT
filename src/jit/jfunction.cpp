#include "jfunction.h"
#include "jhelper.h"

using namespace JIT;

JFunction::JFunction(SafeStatement const& stmt) {
  prepare(stmt);
}

JFunction::~JFunction() {
  Helper::freeFunctionPointer(_storedFn, _fnSize);
}

void JFunction::prepare(SafeStatement const& stmt) {
  ByteBuffer buffer;

  Helper::insertPrologue(buffer);
  stmt->write(buffer);
  Helper::insertEpilogue(buffer);
  
  _storedFn = Helper::prepareFunctionPointer(buffer);
  _fnSize = buffer.current();
}

int64_t JFunction::run() {
  return _storedFn();
}
