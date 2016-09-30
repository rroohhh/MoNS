#ifndef _PLATFORM_H
#define _PLATFORM_H

#include "../simulation/Simtypes.h"
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

// platform abstractions / helpers

namespace Platform {
    template <typename T>
    inline static T * allocate(Simtypes::SIZE size) noexcept {
        return (T *)malloc(size * sizeof(T));
    }

    template <typename T>
    inline static T * allocateInit(Simtypes::SIZE size) noexcept {
        return (T *)calloc(size, sizeof(T));
    }

    template <typename T>
    inline static void * reallocate(T * src, Simtypes::SIZE size) noexcept {
        return (T *)realloc((void *)src, size * sizeof(T));
    }

    template <typename T>
    inline static void copy(T * src, T * dst, Simtypes::SIZE count) noexcept {
        memcpy((void *)dst, (void *)src, count * sizeof(T));
    }

    template <typename T>
    inline static void deallocate(T * target) noexcept {
        return free((void *)target);
    }

    inline static int fork() {
        auto ret = ::fork();
        if(!ret) {
            struct sigaction act;
            act.sa_handler = SIG_DFL;
            sigemptyset(&act.sa_mask);
            act.sa_flags = 0;
            sigaction(SIGINT, &act, 0);
        }

		return ret;
    }
}

#endif
