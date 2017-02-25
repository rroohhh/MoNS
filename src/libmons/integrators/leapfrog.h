#ifndef __LEAPFROG_H_
#define __LEAPFROG_H_

#include "../simulation/simulation_data.h"
#include "types.h"

namespace mons {
    class leapfrog {
    public:
        leapfrog(simulation_data * data) noexcept;

        f step(f simTime) noexcept;
    private:
        void init(v3 * velocities) noexcept;

        f *    dt;
        size * bodycount;
        v3 *   positions;
        v3 *   old_positions;
        v3 *   acceleration;
    };
}

#endif
