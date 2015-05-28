#include "jfreference.h"

using namespace JIT;

FunctionReference::FunctionReference() {
  _fn = nullptr;
}

FunctionReference::FunctionReference(SafeFunction fn) {
  set(fn);
}

SafeFunction FunctionReference::get() const {
  return _fn;
}

void FunctionReference::set(SafeFunction fn) {
  _fn = fn;
}
	
SafeFunctionReference JIT::makeFunctionReference() {
  return SafeFunctionReference(new FunctionReference());
}
