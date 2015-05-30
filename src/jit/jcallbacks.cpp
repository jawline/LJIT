#include "jcallbacks.h"
#include <stdio.h>

using namespace JIT;

int64_t Callbacks::print(int64_t val) {
  printf("Print Callback: %li\n", val);
  return val;
}

int64_t Callbacks::unresolved() {
  printf("I am a unresolved function call. Something has gone wrong\n");
  return 0;
}

int64_t Callbacks::set(int64_t idx, int64_t val) {
	return val;
}

int64_t Callbacks::get(int64_t idx) {
	return 5;
}
