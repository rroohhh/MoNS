/*
  Copyright (C) 2017 by Robin Heinemann. All Rights Reserved.
  rdtsc.h -- rdtsc access
*/

#ifndef __RDTSC_H_
#define __RDTSC_H_

#include "../types.h"

namespace mons {
    namespace platform {

#ifdef __i386
        inline u64 rdtsc() {
            u64 x;
            asm volatile("rdtsc" : "=A"(x));
            return x;
        }
#elif defined __amd64
        inline u64 rdtsc() {
            u64 a, d;
            asm volatile("rdtsc" : "=a"(a), "=d"(d));
            return (d << 32) | a;
        }
#endif
    }
}

#endif
