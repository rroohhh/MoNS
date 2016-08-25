#ifndef _PLATFORM_H
#define _PLATFORM_H

#include "../simulation/Simtypes.h"
#include <cstdlib>
#include <cstring>

// platform abstractions / helpers

namespace Platform {
    template <typename T>
    inline static T * allocate(Simtypes::SIZE size) {
        return (T *)malloc(size * sizeof(T));
    }

    template <typename T>
    inline static T * allocateInit(Simtypes::SIZE size) {
        return (T *)calloc(size, sizeof(T));
    }

    template <typename T>
    inline static void * reallocate(T * src, Simtypes::SIZE size) {
        return (T *)realloc((void *)src, size * sizeof(T));
    }

    template <typename T>
    inline static void copy(T * src, T * dst, Simtypes::SIZE count) {
        memcpy((void *)dst, (void *)src, count * sizeof(T));
    }

    template <typename T>
    inline static void deallocate(T * target) {
        return free((void *)target);
    }
};

#endif
