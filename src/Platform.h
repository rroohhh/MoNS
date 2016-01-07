#ifndef _PLATFORM_H
#define _PLATFORM_H

#include <cstdlib>
#include "Simtypes.h"

namespace Platform {
	inline static void * allocate(Simtypes::SIZE size) {
		return malloc(size);
	}

	inline static void * reallocate(void * src, Simtypes::SIZE size) {
		return realloc(src, size);
	}

	inline static void * deallocate(void * target) {
		return free(target);
	}
};


#endif
