#ifndef _JIT_FUNCTION_DEF_H_
#define _JIT_FUNCTION_DEF_H_
#include "../utils/bytebuffer.h"
#include <string>
#include <vector>
using namespace std;

namespace JIT {

class JFunction {
private:
	int64_t _val;

public:
	JFunction(int64_t x);
	~JFunction();

	void write(Assembler::ByteBuffer& buffer);
};

}

#endif //_JIT_FUNCTION_DEF_H_