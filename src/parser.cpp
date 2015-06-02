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

bool Parser::resolveAll() {
	for (unsigned int i = 0; i < _unresolved.size(); i++) {
		if (_functions.find(_unresolved[i].first) != _functions.end()) {
			if (_unresolved[i].second->getNumArgs() != _functions[_unresolved[i].first]->getNumArgs()) {
				printf("Num args differs from expected\n");
				return false;
			}
			_unresolved[i].second->updateCallback((void*)_functions[_unresolved[i].first]->getFnPtr());
			_unresolved.erase(_unresolved.begin() + i);
			return resolveAll();
		}
	}
	return true;
}

SafeStatement Parser::parseFunctionCall(char const*& input, std::vector<std::string> const& argList) {

	//Get function call name
	Token next = _tokeniser.nextToken(input);

	string name = next.asString();

	std::vector<SafeStatement> args;

	while (true) {
		next = _tokeniser.peekToken(input);
		
		if (next.id() == LPAREN) {
			SafeStatement arg = parseBlock(input, argList);
			CHECK(arg);
			args.push_back(arg);
		} else if (next.id() == NUM) {
			SafeStatement arg = parseAtom(input);
			CHECK(arg);
			args.push_back(arg);
		} else if (next.id() == ID) {
			//Peek the name
			next = _tokeniser.peekToken(input);

			SafeStatement arg = nullptr;

			if (getArg(next.asString(), argList) != -1) {
				arg = parseArg(input, argList);
			} else {
				printf("Cannot reach %s in this call (Perhaps it needs to be surrounded by parenthesis)\n", next.asString());
			}

			CHECK(arg);
			args.push_back(arg);
		} else {
			break;
		}
	}
	
	StatementType type;
	void* callback = nullptr;
	int numExpectedArgs;
	
	if (name.compare("+") == 0) {
		type = Add;
		numExpectedArgs = 2;
	} else if (name.compare("-") == 0) {
		type = Subtract;
		numExpectedArgs = 2;
	} else if (name.compare("*") == 0) {
		type = Multiply;
		numExpectedArgs = 2;
	} else if (name.compare("/") == 0) {
		type = Divide;
		numExpectedArgs = 2;
	} else if (name.compare("if") == 0) {
		type = If;
		numExpectedArgs = 3;
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
		numExpectedArgs = -1;
	}
	
	if (numExpectedArgs != -1 && args.size() != numExpectedArgs) {
		printf("Expected %i args got %li on %s\n", numExpectedArgs, args.size(), name.c_str());
		return nullptr;
	}

	auto result = SafeStatement(new Statement(type, callback, args));

	if (type == NativeCallback && callback == nullptr) {
		_unresolved.push_back(pair<string, SafeStatement>(name, result));
	}

	return result;
}

int Parser::getArg(std::string arg, std::vector<std::string> const& argList) {
	for (unsigned int i = 0; i < argList.size(); i++) {
		if (argList[i].compare(arg) == 0) {
			return i;
		}
	}
	return -1;
}

JIT::SafeStatement Parser::parseArg(char const*& input, std::vector<std::string> const& argList) {
	Token next = _tokeniser.nextToken(input);
	return SafeStatement(new Statement(Arg, getArg(next.asString(), argList)));
}

SafeStatement Parser::parseBlock(char const*& input, std::vector<std::string> const& argList) {

	//Discard lparen
	Token next = _tokeniser.nextToken(input);
	next = _tokeniser.peekToken(input);
	SafeStatement result;

	if (next.id() == ID) {

		//Peek the name
		next = _tokeniser.peekToken(input);

		if (getArg(next.asString(), argList) != -1) {
			result = parseArg(input, argList);
		} else {
			result = parseFunctionCall(input, argList);
		}

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

bool Parser::parseFunctionArguments(char const*& input, std::vector<std::string>& argList) {
	
	next = _tokeniser.nextToken(input);
	
	if (next.id() != ID) {
		argList.push_back(next.asString());
	}
	
	argList.push_back(next.asString());
	
	next = _tokeniser.nextToken(input);
	
	if (next.id() == RPAREN) {
		return true;
	} else if (next.id() == COMMA) {
		return parseFunctionArguments(input, argList);
	} else {
		print("Expected RPAREN or COMMA\n");
		return false;
	}
}

bool Parser::parseFunction(char const*& input, std::map<std::string, SafeFunction>& functionList) {
	
	Token next = _tokeniser.nextToken(input);
	next = _tokeniser.nextToken(input);
	
	if (next.id() != ID) {
		printf("Expected function name\n");
		return false;
	}

	std::string name = next.asString();

	//Parse args if any are supplied
	std::vector<std::string> args;

	next = _tokeniser.peekToken(input);

	if (next.id() == LPAREN) {
		//discard lparen
		_tokeniser.nextToken(input);
		if (!parseFunctionArguments(input, args)) {
			return false;
		}
	}
		
	next = _tokeniser.nextToken(input);
	
	if (next.id() != ARROW) {
		printf("Expected arrow\n");
		return false;
	}
	
	SafeStatement block = parseBlock(input, args);
	CHECK(block);
	functionList[name] = SafeFunction(new Function(block, args.size()));

	return true;
}

bool Parser::innerParse(char const*& input) {
	
	Token next = _tokeniser.peekToken(input);

	if (next.id() == TOKEN_EOF) {
		return true;
	}

	if (next.id() == LPAREN) {
		SafeStatement block = parseBlock(input, std::vector<std::string>());
		CHECK(block);
		Function fn = Function(block, 0);
		if (!resolveAll()) {
			return false;
		}
		fn.rewriteCallbacks();
		printf("Line Result: %li\n", fn.run());
	} else if (next.id() == FUNCTION) {
		if (!parseFunction(input, _functions)) {
			return false;
		}
		if (!resolveAll()) {
			return false;
		}
		for (auto it = _functions.begin(); it != _functions.end(); it++) {
			it->second->rewriteCallbacks();
		}
	} else {
		printf("Expected LPAREN\n");
		return false;
	}

	return innerParse(input);
}

bool Parser::parse(char const* input) {
	return innerParse(input);
}
