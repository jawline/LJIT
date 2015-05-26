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
}

Token::Token(TOKEN_ID id) {
	_id = id;
	_data = "";
}

Token::Token(TOKEN_ID id, char const* input, size_t len) {
	_id = id;
	_data = std::string(input, len);
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

Tokeniser::Tokeniser() {
	regexParse(&idRegex, "[a-zA-Z][a-zA-Z0-9_]*");
	regexParse(&intRegex, "-?[0-9]+");
}

Tokeniser::~Tokeniser() {
	regexFree(&idRegex);
	regexFree(&intRegex);
}

char const* Tokeniser::skipWhite(char const* input) {
  for (; *input && isspace(*input); input++) {}
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

Token Tokeniser::peekToken(char const* input, size_t& len) {
	Token result = Token(INVALID_TOKEN);
	input = skipWhite(input);

	if (*input == '\0') {
		result = Token(TOKEN_EOF);
		len = 0;
	} else if (strncmp(input, "(", 1) == 0) {
		result = Token(LPAREN, input, 1);
		len = 1;
	} else if (strncmp(input, ")", 1) == 0) {
		result = Token(RPAREN, input, 1);
		len = 1;
	} else if ((len = nfaMatches(idRegex.start, input)) > 0) {
		result = Token(ID, input, len);
	} else if ((len = nfaMatches(intRegex.start, input)) > 0) {
		result = Token(NUM, input, len);
	} else {
		printf("%s is invalid\n", input);
	}

	return result;
}
