/*
  Copyright (C) 2017 by Robin Heinemann. All Rights Reserved.
  gravity_naive.h -- naive gravity implementation
*/

#ifndef __GRAVITY_H_
#define __GRAVITY_H_

#include "../simulation/simulation_data.h"
#include "bench.h"
#include "constants.h"
#include "types.h"

#include <immintrin.h>

// ToDo(robin): collision detection

namespace mons {
    class gravity_naive {
    public:
        gravity_naive(simulation_data * data) noexcept;

        /*
         * __attribute__((target("default")))
         * void apply(f simTime) noexcept;
         */

        /* __attribute__((target("avx2"))) */
        void apply(f simTime) noexcept;

    private:
        void apply() noexcept;

        size bodycount;
        f * __restrict__ mass;

        f * __restrict__ px;
        f * __restrict__ py;
        f * __restrict__ pz;

        f * __restrict__ ax;
        f * __restrict__ ay;
        f * __restrict__ az;
    };
}

#endif
