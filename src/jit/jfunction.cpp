#include "jfunction.h"
#include "jhelper.h"

using namespace JIT;
using namespace Assembler;

Function::Function(SafeStatement const& stmt) {
  prepare(stmt);
}

Function::~Function() {
  Helper::freeFunctionPointer(_storedFn, _fnSize);
}

void Function::prepare(SafeStatement const& stmt) {
  ByteBuffer buffer;

  Helper::insertPrologue(buffer);
  stmt->write(buffer);
  Helper::insertEpilogue(buffer);
  
  _storedFn = Helper::prepareFunctionPointer(buffer);
  _fnSize = buffer.current();
}

int64_t Function::run(Scope* scope) const {
  return _storedFn(scope);
}
