#ifndef _COREASM_PARSER_DEF_H_
#define _COREASM_PARSER_DEF_H_
#include "utils/bytebuffer.h"
#include "tokens.h"
#include <string>
#include <vector>

using namespace std;

namespace Assembler {
	class Parser {
	private:
		Tokeniser _tokeniser;

		bool postParse(ByteBuffer& buffer);
		bool parseAtom(char const*& input, ByteBuffer& buffer);
		bool parseFunctionCall(char const*& input, ByteBuffer& buffer);
		bool parseBlock(char const*& input, ByteBuffer& buffer);

	public:
		Parser();
		~Parser();

		bool parse(char const* input, ByteBuffer& buffer);
	};
}

#endif //_COREASM_PARSER_DEF_H_
