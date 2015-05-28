#ifndef _JIT_FUNCTION_REFERENCE_DEF_H_
#define _JIT_FUNCTION_REFERENCE_DEF_H_
#include "jfunction.h"

namespace JIT {
	
	class FunctionReference {
	private:
		SafeFunction _fn;
	public:
		FunctionReference();
		FunctionReference(SafeFunction fn);
		SafeFunction get() const;
		void set(SafeFunction fn);
	};
	
	typedef std::shared_ptr<FunctionReference> SafeFunctionReference;
	SafeFunctionReference makeFunctionReference();
}

#endif
