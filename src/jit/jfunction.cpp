#include "jfunction.h"
#include "jhelper.h"

using namespace JIT;
using namespace Assembler;

Function::Function(SafeStatement const& stmt) {
  _storedFn = nullptr;
  _stmt = stmt;
}

Function::~Function() {
  if (_storedFn) {
  	Helper::freeFunctionPointer(_storedFn, _fnSize);
  }
}

void Function::prepare(SafeStatement const& stmt) {
  ByteBuffer buffer;

  Helper::insertPrologue(buffer);
  stmt->write(buffer);
  Helper::insertEpilogue(buffer);
  
  _storedFn = Helper::prepareFunctionPointer(buffer);
  _fnSize = buffer.current();
}

void Function::resolveAll() {
	if (!_storedFn) {
		return;
	}

	for (unsigned int i = 0; i < _unresolvedCallList.size(); i++) {
		if (!_unresolvedCallList[i].first->getCallback()) {
			printf("STILL NOT ABLE TO RESOLVE ADDRESS\n");
		} else {
			printf("HEHEHEHE RESOLVED\n");
			Helper::updateAddress(_storedFn, _unresolvedCallList[i].second, _unresolvedCallList[i].first->getCallback());
		}
	}
}

int64_t Function::run(Scope* scope) {
  return getFnPtr()(scope);
}

JFPTR Function::getFnPtr() {
	if (!_storedFn) {
		prepare(_stmt);
	}
	return _storedFn;
}
