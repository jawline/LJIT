#include "bytebuffer.h"
#include <string.h>

using namespace Assembler;

ByteBuffer::ByteBuffer() {
	buffer = new uint8_t[cDefaultBufferSize];
	bufferCurrentIter = 0;
	bufferCurrentSize = cDefaultBufferSize;
}

ByteBuffer::~ByteBuffer() {
	delete[] buffer;
}

void ByteBuffer::expandBuffer() {
	uint8_t* newBuffer = new uint8_t[bufferCurrentSize + cDefaultBufferSize];
	memcpy(newBuffer, buffer, bufferCurrentIter);
	delete[] buffer;
	buffer = newBuffer;
	bufferCurrentSize += cDefaultBufferSize;
}

void ByteBuffer::insert(uint8_t byte) {
	if (bufferCurrentIter + 1 >= bufferCurrentSize) {
		expandBuffer();
	}
	insert(byte, bufferCurrentIter);
	bufferCurrentIter += 1;
}

void ByteBuffer::insert(uint16_t word) {
	if (bufferCurrentIter + sizeof(uint16_t) >= bufferCurrentSize) {
		expandBuffer();
	}
	insert(word, bufferCurrentIter);
	bufferCurrentIter += sizeof(uint16_t);
}

void ByteBuffer::insert(uint32_t num) {
	if (bufferCurrentIter + sizeof(uint32_t) >= bufferCurrentSize) {
		expandBuffer();
	}
	insert(num, bufferCurrentIter);
	bufferCurrentIter += sizeof(uint32_t);
}


void ByteBuffer::insert(uint8_t byte, size_t at) {
	buffer[at] = byte;
}

void ByteBuffer::insert(uint16_t word, size_t at) {
	*((uint16_t*)&buffer[at]) = word;
}

void ByteBuffer::insert(uint32_t num, size_t at) {
	*((uint32_t*)&buffer[at]) = num;
}

uint8_t* ByteBuffer::raw() const {
	return buffer;
}

size_t ByteBuffer::current() const {
	return bufferCurrentIter;
}
