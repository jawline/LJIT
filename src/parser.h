#ifndef _COREASM_PARSER_DEF_H_
#define _COREASM_PARSER_DEF_H_
#include "utils/bytebuffer.h"
#include "jit/jfunction.h"
#include "tokens.h"
#include <string>
#include <vector>
#include <map>

using namespace std;

namespace Assembler {
	class Parser {
	private:
		Tokeniser _tokeniser;
		map<string, JIT::SafeFunction> _functions;
		vector<pair<string, JIT::SafeStatement>> _unresolved;
		JIT::SafeStatement parseAtom(char const*& input);
		JIT::SafeStatement parseFunctionCall(char const*& input);
		JIT::SafeStatement parseBlock(char const*& input);
		void resolveAll();
		bool parseFunction(char const*& input, map<string, JIT::SafeFunction>& functionList);
		bool innerParse(char const*& input, JIT::Scope* scope);
	public:
		Parser();
		~Parser();

		bool parse(char const* input);
	};
}

#endif //_COREASM_PARSER_DEF_H_
