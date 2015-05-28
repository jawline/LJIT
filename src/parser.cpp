#include "parser.h"
#include <CoreVM/instructions.h>
#include <CoreVM/registers.h>
#include <vector>
#include <stdio.h>
#include "jit/jcallbacks.h"

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

void Parser::resolveAll() {
	for (unsigned int i = 0; i < _unresolved.size(); i++) {
		if (_functions.find(_unresolved[i].first) != _functions.end()) {
			_unresolved[i].second->updateCallback((void*)_functions[_unresolved[i].first]->getFnPtr());
			_unresolved.erase(_unresolved.begin() + i);
			resolveAll();
			return;
		}
	}
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
	void* callback = nullptr;
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
		type = NativeCallback;
		callback = (void*)Callbacks::set;
		numExpectedArgs = 2;
	} else if (name.compare("get") == 0) {
		type = NativeCallback;
		callback = (void*)Callbacks::get;
		numExpectedArgs = 1;
	} else if (name.compare("print") == 0) {
		type = NativeCallback;
		callback = (void*)Callbacks::print;
		numExpectedArgs = 1;
	} else {
		type = NativeCallback;
		callback = nullptr;
		numExpectedArgs = 0;
	}
	
	if (args.size() != numExpectedArgs) {
		printf("Expected %i args got %li\n", numExpectedArgs, args.size());
		return nullptr;
	}

	auto result = SafeStatement(new Statement(type, callback, args));

	if (type == NativeCallback && callback == nullptr) {
		_unresolved.push_back(pair<string, SafeStatement>(name, result));
	}

	resolveAll();

	return result;
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

bool Parser::parseFunction(char const*& input, std::map<std::string, SafeFunction>& functionList) {
	
	Token next = _tokeniser.nextToken(input);
	next = _tokeniser.nextToken(input);
	
	if (next.id() != ID) {
		printf("Expected function name\n");
		return false;
	}
	
	std::string name = next.asString();
	
	next = _tokeniser.nextToken(input);
	
	if (next.id() != ARROW) {
		printf("Expected arrow\n");
		return false;
	}
	
	SafeStatement block = parseBlock(input);
	CHECK(block);
	functionList[name] = SafeFunction(new Function(block));

	printf("Stored function %s\n", name.c_str());

	return true;
}

bool Parser::innerParse(char const*& input, JIT::Scope* scope) {
	
	Token next = _tokeniser.peekToken(input);

	if (next.id() == TOKEN_EOF) {
		return true;
	}

	if (next.id() == LPAREN) {
		SafeStatement block = parseBlock(input);
		CHECK(block);
		Function fn = Function(block);
		printf("Line Result: %li\n", fn.run(scope));
	} else if (next.id() == FUNCTION) {
		if (!parseFunction(input, _functions)) {
			return false;
		}
		resolveAll();
	} else {
		printf("Expected LPAREN\n");
		return false;
	}

	return innerParse(input, scope);
}

bool Parser::parse(char const* input) {
	Scope* scope = new Scope();
	bool res = innerParse(input, scope);
	delete scope;
	return res;
}
