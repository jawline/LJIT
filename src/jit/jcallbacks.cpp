#include "jcallbacks.h"
#include <stdio.h>

using namespace JIT;

int64_t Callbacks::print(Scope* scope, int64_t val) {
  printf("Print Callback: %li\n", val);
  return val;
}

int64_t Callbacks::unresolved(Scope* scope) {
  printf("I am a unresolved function call. Something has gone wrong\n");
  return 0;
}

int64_t Callbacks::set(Scope* scope, int64_t idx, int64_t val) {
  scope->set(idx, val);
  return val;
}

int64_t Callbacks::get(Scope* scope, int64_t idx) {
  return scope->get(idx);
}
