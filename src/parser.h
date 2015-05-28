#ifndef _COREASM_PARSER_DEF_H_
#define _COREASM_PARSER_DEF_H_
#include "utils/bytebuffer.h"
#include "jit/jfreference.h"
#include "tokens.h"
#include <string>
#include <vector>
#include <map>

using namespace std;

namespace Assembler {
	class Parser {
	private:
		Tokeniser _tokeniser;
		std::map<std::string, JIT::FunctionReference> _functions;
		JIT::SafeStatement parseAtom(char const*& input);
		JIT::SafeStatement parseFunctionCall(char const*& input);
		JIT::SafeStatement parseBlock(char const*& input);
		bool parseFunction(char const*& input, std::map<std::string, JIT::FunctionReference>& functionList);
	public:
		Parser();
		~Parser();

		bool parse(char const* input);
	};
}

#endif //_COREASM_PARSER_DEF_H_
