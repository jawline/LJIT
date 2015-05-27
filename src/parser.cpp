#include "parser.h"
#include <CoreVM/instructions.h>
#include <CoreVM/registers.h>
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

	int args = 0;

	while (true) {
		next = _tokeniser.peekToken(input);
		
		if (next.id() == LPAREN) {
			if (!parseBlock(input, buffer)) {
				return nullptr;
			}
		} else if (next.id() == NUM) {
			if (!parseAtom(input, buffer)) {
				return nullptr;
			}
		} else {
			break;
		}

		args++;
	}

	printf("call %s: %i args\n", name.c_str(), args);
	return true;
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
