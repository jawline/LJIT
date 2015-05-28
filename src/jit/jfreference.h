#ifndef _JIT_FUNCTION_REFERENCE_DEF_H_
#define _JIT_FUNCTION_REFERENCE_DEF_H_

namespace JIT {
	class FunctionReference {
		private:
			SafeFunction _fn;
		public:
			FunctionReference();
			FunctionReference(SafeFunction fn);
			
			SafeFunction get() const;
	}
}

#endif
