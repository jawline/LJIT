#ifndef _JIT_FUNCTION_DEF_H_
#define _JIT_FUNCTION_DEF_H_
#include "../utils/bytebuffer.h"
#include "jstatement.h"

namespace JIT {
	class JFunction {
	private:
	public:
		JFunction(JStatement stmt);
		~JFunction();
	};
}

#endif //_JIT_FUNCTION_DEF_H_
