#include "jhelper.h"
#include "jtemplate.h"

using namespace JIT;

void Helper::insertPrologue(ByteBuffer& buffer) {
  buffer.insert(Template::prologue, sizeof(Template::prologue));
}

void Helper::insertEpilogue(ByteBuffer& buffer) {
  buffer.insert(Template::epilogue, sizeof(Template::epilogue));
}

void Helper::pushNumber(int64_t value, ByteBuffer& buffer) {
}

void Helper::addTopTwoStack(ByteBuffer& buffer) {
}
