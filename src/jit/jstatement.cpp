#include "jstatement.h"
#include "jhelper.h"
#include "jcallbacks.h"

using namespace JIT;

Statement::Statement(int64_t val) {
  _type = Atom;
  _val = val;
}

Statement::Statement(StatementType type, size_t argNum) {
  _type = type;
  _argNum = argNum;
}

Statement::Statement(StatementType type, std::vector<SafeStatement> const& args) {
  _type = type;
  _args = args;
}

Statement::Statement(StatementType type, void* callback, std::vector<SafeStatement> const& args) {
  _type = type;
  _callback = callback;
  _args = args;
}

void Statement::updateCallback(void* callback) {
  _callback = callback;
}

void* Statement::getCallback() const {
  return _callback;
}

int Statement::getNumArgs() const {
  return _args.size();
}

void Statement::write(Assembler::ByteBuffer& buffer, std::vector<std::pair<Statement*, size_t>>& unresolvedList) {
  switch (_type) {
    case Atom:
      Helper::pushNumber(_val, buffer);
      break;
    case Add:
      _args[0]->write(buffer, unresolvedList);
      _args[1]->write(buffer, unresolvedList);
      Helper::addTopTwoStack(buffer);
      break;
    case Subtract:
      _args[0]->write(buffer, unresolvedList);
      _args[1]->write(buffer, unresolvedList);
      Helper::subTopTwoStack(buffer);
      break;
    case Multiply:
      _args[0]->write(buffer, unresolvedList);
      _args[1]->write(buffer, unresolvedList);
      Helper::mulTopTwoStack(buffer);
      break;
    case Divide:
      _args[0]->write(buffer, unresolvedList);
      _args[1]->write(buffer, unresolvedList);
      Helper::divTopTwoStack(buffer);
      break;
    case Arg:
      printf("I don't know how to get args yet\n");
      break;
    case If: {
      
      //Execute condition
      _args[0]->write(buffer, unresolvedList);
      
      //Write a jump instruction with BS location
      size_t elseAddr = Helper::jumpRelativeTopEqualZero(buffer, 0xDEAD);
      _args[1]->write(buffer, unresolvedList);
      
      //Write a jump location to avoid the else
      size_t exitAddr = Helper::jumpRelative(buffer, 0xDEAD);
      
      size_t elseLocation = buffer.current();
      
      if (_args.size() == 3) {
        _args[2]->write(buffer, unresolvedList);
      } else {
        //The there is no else will just push the number 0
        Helper::pushNumber(0, buffer);
      }
      
      //Rewrite the dummy relative locations to be the actual exit
      size_t exitLocation = buffer.current();
      auto elseJmpNextInstruction = elseAddr + sizeof(int32_t);
      auto exitJmpNextInstruction = exitAddr + sizeof(int32_t);

      //TODO: Dirty? Nasty! Make do nice?
      buffer.insert((int32_t)(elseLocation - elseJmpNextInstruction), elseAddr);
      buffer.insert((int32_t)(exitLocation - exitJmpNextInstruction), exitAddr);
      break;
    }
    case NativeCallback: {
      Helper::setArgumentZeroScope(buffer);

      //TODO: SCARY! This will break with over 6 args, work out a nice way to do this
      printf("TODO: NativeCallback arg set is broken with over 6 args\n");
      for (unsigned int i = 0; i < _args.size(); i++) {
        _args[i]->write(buffer, unresolvedList);
      }
      for (int i = _args.size(); i > 0; i--) {
        Helper::setArgumentStackTop(i, buffer);
      }
      size_t addressStart = Helper::callFunction(_callback ? _callback : ((void*)Callbacks::unresolved), buffer);
      if (_callback == nullptr) {
        printf("Cannot produce nativecall, _callback unresolved\n");
        printf("Adding to unresolved list\n");
        unresolvedList.push_back(std::pair<Statement*, size_t>(this, addressStart));
      }
      break;
    }
    default:
      printf("Could not JIT\n");
      break;
  }
}
