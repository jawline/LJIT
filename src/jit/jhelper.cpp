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
  //mov _val, RAX
	uint8_t mrax[2] = { 0x48, 0xB8 };
	buffer.insert(mrax, 2);
	buffer.insert((int64_t) _val);
	
	//push RAX
	buffer.insert((uint8_t)0x50);
}

void Helper::addTopTwoStack(ByteBuffer& buffer) {
  //pop RCX
  //pop RAX
  //add RCX, RAX
  //push RAX
}
