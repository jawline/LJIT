#include "jscope.h"
#include <stdio.h>

using namespace JIT;

Scope::Scope() {
  _data = new int64_t[250];
  for (unsigned int i = 0; i < 250; i++) {
    _data[i] = 0;
  }
}

Scope::~Scope() {
  delete[] _data;
}

void Scope::set(int64_t idx, int64_t val) {
  if (idx < 0 || idx >= 250) {
    printf("IDX %li out of bounds (set)\n", idx);
    return;
  }
  _data[idx] = val;
}

int64_t Scope::get(int64_t idx) const {
  if (idx < 0 || idx >= 250) {
    printf("IDX %li out of bounds (get)\n", idx);
    return 0;
  }
  return _data[idx];
}
