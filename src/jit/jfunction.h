#ifndef _JIT_FUNCTION_DEF_H_
#define _JIT_FUNCTION_DEF_H_
#include "../utils/bytebuffer.h"
#include "jstatement.h"
#include "jhelper.h"

namespace JIT {
	class Function {
	private:
		void prepare(SafeStatement const& stmt);
		JFPTR _storedFn;
		size_t _fnSize;

	public:
		Function(SafeStatement const& stmt);
		~Function();
		
		int64_t run() const;
	};
}

#endif //_JIT_FUNCTION_DEF_H_
