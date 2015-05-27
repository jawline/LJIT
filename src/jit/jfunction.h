#ifndef _JIT_FUNCTION_DEF_H_
#define _JIT_FUNCTION_DEF_H_
#include "../utils/bytebuffer.h"
#include "jstatement.h"
#include "jhelper.h"

namespace JIT {
	class JFunction {
	private:
		void prepare(JStatement  const& stmt);
		JFPTR _storedFn;
		size_t _fnSize;

	public:
		JFunction(JStatement stmt);
		~JFunction();
	};
}

#endif //_JIT_FUNCTION_DEF_H_
