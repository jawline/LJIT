#ifndef _JTEMPLATE_DEF_H_
#define _JTEMPLATE_DEF_H_
#include <cstdint>

namespace JIT {
	class Template {
	public:
		static uint8_t prologue[];
		static uint8_t epilogue[];
	};
}

#endif //_JTEMPLATE_DEF_H_