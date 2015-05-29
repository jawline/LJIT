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
		std::vector<std::pair<Statement*, size_t>> _unresolvedCallList;
		size_t _fnSize;
		SafeStatement _stmt;

	public:
		Function(SafeStatement const& stmt);
		~Function();
		
		int64_t run(Scope* scope);
		JFPTR getFnPtr();
	};
	
	typedef std::shared_ptr<Function> SafeFunction;
}

#endif //_JIT_FUNCTION_DEF_H_
