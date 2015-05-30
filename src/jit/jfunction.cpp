#include "jfunction.h"
#include "jhelper.h"

using namespace JIT;
using namespace Assembler;

Function::Function(SafeStatement const& stmt, size_t numArgs) {
  _storedFn = nullptr;
  _stmt = stmt;
  _numArgs = numArgs;
}

Function::~Function() {
  if (_storedFn) {
  	Helper::freeFunctionPointer(_storedFn, _fnSize);
  }
}

void Function::prepare(SafeStatement const& stmt) {
  ByteBuffer buffer;

  Helper::insertPrologue(buffer);

  //Push all the args so they sit left to right from ebp
  Helper::functionEntryPushArgs(_numArgs, buffer);
  stmt->write(buffer, _unresolvedCallList);
  Helper::functionExitDiscardArgs(_numArgs, buffer);
  Helper::insertEpilogue(buffer);
  
  _storedFn = Helper::prepareFunctionPointer(buffer);
  _fnSize = buffer.current();
}

size_t Function::getNumArgs() const {
	return _numArgs;
}

void Function::rewriteCallbacks() {

	if (!_storedFn) {
		return;
	}

	for (unsigned int i = 0; i < _unresolvedCallList.size(); i++) {
		if (!_unresolvedCallList[i].first->getCallback()) {
			printf("STILL NOT ABLE TO RESOLVE ADDRESS\n");
		} else {
			printf("RewriteCallbacks resolved a callback\n");
			Helper::updateAddress(_storedFn, _unresolvedCallList[i].second, _unresolvedCallList[i].first->getCallback());
		}
	}
}

int64_t Function::run() {
  return getFnPtr()();
}

JFPTR Function::getFnPtr() {
	if (!_storedFn) {
		prepare(_stmt);
	}
	return _storedFn;
}
