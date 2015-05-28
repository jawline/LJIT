#include "jscope.h"

using namespace JIT;

Scope::Scope() {
  _data = new int64_t[250];
}

Scope::~Scope() {
  delete[] _data;
}

void Scope::set(int64_t idx, int64_t val) {
  if (idx < 0 || idx >= 250) {
    return;
  }
  _data[idx] = val;
}

int64_t Scope::get(int64_t idx) const {
  if (idx < 0 || idx >= 250) {
    return 0;
  }
  return _data[idx];
}
