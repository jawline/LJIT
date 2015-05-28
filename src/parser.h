#ifndef _COREASM_PARSER_DEF_H_
#define _COREASM_PARSER_DEF_H_
#include "utils/bytebuffer.h"
#include "jit/jfunction.h"
#include "tokens.h"
#include <string>
#include <vector>

using namespace std;

namespace Assembler {
	class Parser {
	private:
		Tokeniser _tokeniser;
		JIT::SafeStatement parseAtom(char const*& input);
		JIT::SafeStatement parseFunctionCall(char const*& input);
		JIT::SafeStatement parseBlock(char const*& input);
		void parseFunction(char const*& input, std::map<std::string, JIT::FunctionReference>& functionList);
	public:
		Parser();
		~Parser();

		bool parse(char const* input);
	};
}

#endif //_COREASM_PARSER_DEF_H_
