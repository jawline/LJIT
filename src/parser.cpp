#include "parser.h"
#include <CoreVM/instructions.h>
#include <CoreVM/registers.h>
#include <stdio.h>

using namespace Assembler;

Parser::Parser() {}
Parser::~Parser() {}

bool Parser::postParse(ByteBuffer& buffer) {
	return true;
}

bool Parser::parseAtom(char const*& input, ByteBuffer& buffer) {
	Token next = _tokeniser.nextToken(input);
	return next.id() == NUM;
}

bool Parser::parseFunctionCall(char const*& input, ByteBuffer& buffer) {

	//Get function call name
	Token next = _tokeniser.nextToken(input);

	string name = next.asString();

	int args = 0;

	while (true) {
		next = _tokeniser.peekToken(input);
		
		if (next.id() == LPAREN) {
			if (!parseBlock(input, buffer)) {
				return false;
			}
		} else if (next.id() == NUM) {
			if (!parseAtom(input, buffer)) {
				return false;
			}
		} else {
			break;
		}

		args++;
	}

	printf("call %s: %i args\n", name.c_str(), args);
	return true;
}

bool Parser::parseBlock(char const*& input, ByteBuffer& buffer) {

	//Discard lparen
	Token next = _tokeniser.nextToken(input);
	next = _tokeniser.peekToken(input);

	if (next.id() == ID) {
		if (!parseFunctionCall(input, buffer)) {
			return false;
		}
	} else if (next.id() == NUM) {
		if (!parseAtom(input, buffer)) {
			return false;
		}
	} else {
		printf("Expected ID or NUM in block\n");
		return false;
	}

	next = _tokeniser.nextToken(input);

	if (next.id() != RPAREN) {
		printf("Expected RPAREN got %s\n", next.asString());
		return false;
	}

	return true;
}

bool Parser::parse(char const* input, ByteBuffer& buffer) {
	
	Token next = _tokeniser.peekToken(input);

	if (next.id() == TOKEN_EOF) {
		return postParse(buffer);
	}

	if (next.id() != LPAREN) {
		printf("Expected LPAREN\n");
		return false;
	}

	if (!parseBlock(input, buffer)) {
		return false;
	}
	
	return parse(input, buffer);
}
