#include "parser.h"
#include <CoreVM/instructions.h>
#include <CoreVM/registers.h>
#include <vector>
#include <stdio.h>

using namespace Assembler;
using namespace JIT;

Parser::Parser() {}
Parser::~Parser() {}

#define CHECK(x) if (!x) { return nullptr; }

SafeStatement Parser::parseAtom(char const*& input) {
	Token next = _tokeniser.nextToken(input);
	if (next.id() != NUM) {
		return nullptr;
	}
	return SafeStatement(new Statement(next.asInt()));
}

SafeStatement Parser::parseFunctionCall(char const*& input) {

	//Get function call name
	Token next = _tokeniser.nextToken(input);

	string name = next.asString();

	std::vector<SafeStatement> args;

	while (true) {
		next = _tokeniser.peekToken(input);
		
		if (next.id() == LPAREN) {
			SafeStatement arg = parseBlock(input);
			CHECK(arg);
			args.push_back(arg);
		} else if (next.id() == NUM) {
			SafeStatement arg = parseAtom(input);
			CHECK(arg);
			args.push_back(arg);
		} else {
			break;
		}
	}
	
	StatementType type;

	int numExpectedArgs;
	
	if (name.compare("add") == 0) {
		type = Add;
		numExpectedArgs = 2;
	} else if (name.compare("sub") == 0) {
		type = Subtract;
		numExpectedArgs = 2;
	} else if (name.compare("mul") == 0) {
		type = Multiply;
		numExpectedArgs = 2;
	} else if (name.compare("div") == 0) {
		type = Divide;
		numExpectedArgs = 2;
	} else if (name.compare("set") == 0) {
		type = Set;
		numExpectedArgs = 2;
	} else if (name.compare("get") == 0) {
		type = Get;
		numExpectedArgs = 1;
	} else if (name.compare("print") == 0) {
		type = Print;
		numExpectedArgs = 1;
	} else {
		printf("%s not a valid call\n", name.c_str());
		return nullptr;
	}
	
	if (args.size() != numExpectedArgs) {
		printf("Expected %i args got %li\n", numExpectedArgs, args.size());
		return nullptr;
	}

	return SafeStatement(new Statement(type, args));
}

SafeStatement Parser::parseBlock(char const*& input) {

	//Discard lparen
	Token next = _tokeniser.nextToken(input);
	next = _tokeniser.peekToken(input);
	SafeStatement result;

	if (next.id() == ID) {
		result = parseFunctionCall(input);
	} else if (next.id() == NUM) {
		result = parseAtom(input);
	} else {
		printf("Expected ID or NUM in block\n");
		result = nullptr;
	}

	CHECK(result);

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
		return true;
	}

	if (next.id() != LPAREN) {
		printf("Expected LPAREN\n");
		return false;
	}
	
	SafeStatement block = parseBlock(input);
	CHECK(block);
	JFunction fn = JFunction(block);
	printf("Line Result: %li\n", fn.run());
	return parse(input);
}
