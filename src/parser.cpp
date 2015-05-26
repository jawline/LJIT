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

bool Parser::parseBlock(char const*& input, ByteBuffer& buffer) {

	//Discard lparen
	Token next = _tokeniser.nextToken(input);
	next = _tokeniser.peekToken(input);

	if (next.id() == ID) {
		//Parse function call
	} else if (next.id() == NUM) {
		//Parse 'atom'
	} else {
		printf("Expected ID or NUM in block\n");
		return false;
	}

	next = _tokeniser.nextToken(input);

	if (next.id() != RPAREN) {
		printf("Expected RPAREN\n");
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
