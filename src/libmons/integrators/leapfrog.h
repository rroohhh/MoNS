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
        void init(f * vx, f * vy, f * vz) noexcept;

        f *  dt;
        size bodycount;
        f *  px;
        f *  py;
        f *  pz;
        f *  opx;
        f *  opy;
        f *  opz;
        f *  ax;
        f *  ay;
        f *  az;
    };
}

#endif
