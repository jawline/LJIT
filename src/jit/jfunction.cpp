#include "jfunction.h"

using namespace JIT;

JFunction::JFunction(int64_t val) {
	_val = val;
}
JFunction::~JFunction() {}

void JFunction::write(Assembler::ByteBuffer& buffer) {

	//mov RAX, _val
	uint8_t mrax[2] = { 0x48, 0xB8 };
	buffer.insert(mrax, 2);
	buffer.insert((int64_t) _val);
	
	//push RAX
	buffer.insert((uint8_t)0x50);
}
