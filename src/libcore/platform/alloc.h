/*
  Copyright (C) 2017 by Robin Heinemann. All Rights Reserved.
  alloc.h -- allocation wrapper
*/

#ifndef __ALLOC_H_
#define __ALLOC_H_

#include "../types.h"
#include <cstdlib>
#include <cstring>

namespace mons {
    namespace platform {
        template <typename T>
        inline static T * allocate(usize size) noexcept {
            return (T *)_mm_malloc(size * sizeof(T), 32);
        }

        template <typename T>
        inline static T * allocateInit(usize size) noexcept {
            auto p = (T *)_mm_malloc(size * sizeof(T), 32);
			memset(p, 0, size * sizeof(T));
            return p;
        }

        /*
		 * template <typename T>
         * inline static void * reallocate(T * src, usize size) noexcept {
         *     return (T *)realloc((void *)src, size * sizeof(T));
         * }
		 */

        template <typename T>
        inline static void copy(T * src, T * dst, usize count) noexcept {
            memcpy((void *)dst, (void *)src, count * sizeof(T));
        }

        template <typename T>
        inline static void deallocate(T * target) noexcept {
            return _mm_free((void *)target);
        }
    }
}

#endif
