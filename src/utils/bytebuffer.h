#ifndef _PARSER_BYTE_BUFFER_DEF_H_
#define _PARSER_BYTE_BUFFER_DEF_H_
#include <cstdint>
#include <cstddef>

namespace Assembler {
	class ByteBuffer {
	private:
		const static unsigned long cDefaultBufferSize = 2048;

		uint8_t* buffer;
		size_t bufferCurrentIter;
		size_t bufferCurrentSize;

		void expandBuffer();

	public:
		ByteBuffer();
		~ByteBuffer();

		void insert(uint8_t byte);
		void insert(uint16_t word);
		void insert(uint32_t num);
		void insert(uint8_t byte, size_t at);
		void insert(uint16_t word, size_t at);
		void insert(uint32_t num, size_t at);

		uint8_t* raw() const;
		size_t current() const;
	};
}

#endif //_PARSER_BYTE_BUFFER_DEF_H_