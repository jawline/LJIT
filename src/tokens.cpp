#include "tokens.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>

extern "C" {
	#include <regexpm/match.h>
}

using namespace Assembler;

Token::Token() {
	_id = INVALID_TOKEN;
	_line = 0;
}

Token::Token(TOKEN_ID id, unsigned int line) {
	_id = id;
	_data = "";
	_line = line;
}

Token::Token(TOKEN_ID id, char const* input, size_t len, unsigned int line) {
	_id = id;
	_data = std::string(input, len);
	_line = line;
}

Token::~Token() {}

TOKEN_ID Token::id() const {
	return _id;
}

char const* Token::asString() const {
	if (_id == INVALID_TOKEN) {
		return "invalid";
	} else if (_id == TOKEN_EOF) {
		return "eof";
	}
	return _data.c_str();
}

int Token::asInt() const {
	return atoi(_data.c_str());
}

unsigned int Token::line() const {
	return _line;
}

std::string Token::debugInfo() const {
	return std::string(asString()) + " on line " + std::to_string(line());
}

Tokeniser::Tokeniser() {
	regexParse(&idRegex, "[a-zA-Z][a-zA-Z0-9_]*");
	regexParse(&intRegex, "-?[0-9]+");
}

Tokeniser::~Tokeniser() {
	regexFree(&idRegex);
	regexFree(&intRegex);
}

char const* Tokeniser::skipWhite(char const* input) {
  for (; *input && isspace(*input); input++) {
  	if (*input == '\n') {
  		_numProcessedLines++;
  	}
  }
  return input;
}

Token Tokeniser::nextToken(char const*& input) {
	input = skipWhite(input);
	size_t len;
	Token next = peekToken(input, len);
	input += len;
	return next;
}

Token Tokeniser::peekToken(char const* input) {
	input = skipWhite(input);
	size_t len;
	return peekToken(input, len);
}

void Tokeniser::resetLines() {
	_numProcessedLines = 0;
}

Token Tokeniser::peekToken(char const* input, size_t& len) {
	Token result = Token(INVALID_TOKEN, getCurrentLine());
	input = skipWhite(input);

	if (*input == '\0') {
		result = Token(TOKEN_EOF, getCurrentLine());
		len = 0;
	} else if (strncmp(input, "(", 1) == 0) {
		result = Token(LPAREN, input, 1, getCurrentLine());
		len = 1;
	} else if (strncmp(input, ")", 1) == 0) {
		result = Token(RPAREN, input, 1, getCurrentLine());
		len = 1;
	} else if (strncmp(input, "->", 2) == 0) {
		result = Token(ARROW, input, 2, getCurrentLine());
		len = 2;
	} else if (strncmp(input, ",", 1) == 0) {
		result = Token(COMMA, input, 1, getCurrentLine());
		len = 1;
	} else if (strncmp(input, "function", 8) == 0) {
		result = Token(FUNCTION, input, 8, getCurrentLine());
		len = 8;
	} else if (strncmp(input, "+", 1) == 0) {
		result = Token(ID, input, 1, getCurrentLine());
		len = 1;
	} else if (strncmp(input, "-", 1) == 0) {
		result = Token(ID, input, 1, getCurrentLine());
		len = 1;
	} else if (strncmp(input, "*", 1) == 0) {
		result = Token(ID, input, 1, getCurrentLine());
		len = 1;
	} else if (strncmp(input, "/", 1) == 0) {
		result = Token(ID, input, 1, getCurrentLine());
		len = 1;
	} else if ((len = nfaMatches(idRegex.start, input)) > 0) {
		result = Token(ID, input, len, getCurrentLine());
	} else if ((len = nfaMatches(intRegex.start, input)) > 0) {
		result = Token(NUM, input, len, getCurrentLine());
	} else {
		printf("%s is invalid\n", input);
	}
	return result;
}

unsigned int Tokeniser::getCurrentLine() const {
	return _numProcessedLines;
}
