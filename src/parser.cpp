#include "parser.h"
#include <CoreVM/instructions.h>
#include <CoreVM/registers.h>
#include <vector>
#include <stdio.h>

using namespace Assembler;

Parser::Parser() {}
Parser::~Parser() {}

JIT::SafeStatement Parser::parseAtom(char const*& input) {
	Token next = _tokeniser.nextToken(input);
	if (next.id() != NUM) {
		return nullptr;
	}
	return JIT::SafeStatement(new JIT::Statement(next.asInt()));
}

JIT::SafeStatement Parser::parseFunctionCall(char const*& input) {

	//Get function call name
	Token next = _tokeniser.nextToken(input);

	string name = next.asString();

	std::vector<SafeStatement> args;

	while (true) {
		next = _tokeniser.peekToken(input);
		
		if (next.id() == LPAREN) {
			SafeStatement next = parseBlock(input, buffer);
			if (!next) {
				return nullptr;
			}
			args.push_back(next);
		} else if (next.id() == NUM) {
			SafeStatement next = parseAtom(input, buffer);
			if (!next) {
				return nullptr;
			}
			args.push_back(next);
		} else {
			break;
		}
	}
	
	JIT::StatementType type;
	
	if (name.compare("add") == 0) {
		type = JIT::Add;
	} else if (name.compare("sub") == 0) {
		type = JIT::Subtract;
	} else if (name.compare("mul") == 0) {
		type = JIT::Multiply;
	} else if (name.compare("div") == 0) {
		type = JIT::Divide;
	} else {
		printf("%s not a valid call\n", name.c_str());
		return nullptr;
	}
	
	if (args.size() != 2) {
		printf("Expected 2 args\n");
		return nullptr;
	}

	printf("call %s: %i args\n", name.c_str(), args.size());
	return JIT::SafeStatement(new JIT::Statement(type, args[0], args[1]));
}

JIT::SafeStatement Parser::parseBlock(char const*& input) {

	//Discard lparen
	Token next = _tokeniser.nextToken(input);
	next = _tokeniser.peekToken(input);
	SafeStatement result;

	if (next.id() == ID) {
		result = parseFunctionCall(input, buffer);
	} else if (next.id() == NUM) {
		result = parseAtom(input, buffer);
	} else {
		printf("Expected ID or NUM in block\n");
		result = nullptr;
	}

	if (!result) {
		return nullptr;
	}

	next = _tokeniser.nextToken(input);

	if (next.id() != RPAREN) {
		printf("Expected RPAREN got %s\n", next.asString());
		return nullptr;
	}

	return result;
}

bool Parser::parse(char const* input) {
	
	Token next = _tokeniser.peekToken(input);

	if (next.id() == TOKEN_EOF) {
		return postParse(buffer);
	}

	if (next.id() != LPAREN) {
		printf("Expected LPAREN\n");
		return false;
	}
	
	SafeStatement block = parseBlock(input, buffer);
	
	if (!block) {
		return false;
	}

	JIT::JFunction fn = JIT::JFunction(block);
	printf("%i\n", fn.run());
	
	return parse(input, buffer);
}
