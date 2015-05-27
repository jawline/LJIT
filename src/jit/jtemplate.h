#ifndef _JTEMPLATE_DEF_H_
#define _JTEMPLATE_DEF_H_
#include <cstdint>
#include <stddef.h>

namespace JIT {
	class Template {
	public:
		static uint8_t prologue[];
		static uint8_t epilogue[];
		static size_t prologueSize();
		static size_t epilogueSize();
	};
}

#endif //_JTEMPLATE_DEF_H_