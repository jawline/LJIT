#include "jfunction.h"
#include "jhelper.h"

using namespace JIT;

JFunction::JFunction(Statement stmt) {}
JFunction::~JFunction() {}

void JFunction::prepare(Statement const& stmt) {
  ByteBuffer buffer;

  Helper::insertPrologue(buffer);
  stmt.write(buffer);
  Helper::insertEpilogue(buffer);
}
