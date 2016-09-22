#ifndef _LEAPFROG_H
#define _LEAPFROG_H

#include "../simulation/SimulationData.h"

class Leapfrog {
public:
    Leapfrog(SimulationData * data) noexcept;

    Simtypes::FLOAT step(Simtypes::FLOAT simTime) noexcept;

private:
    void init(Simtypes::v3 * velocities) noexcept;

    Simtypes::FLOAT * dt;
    Simtypes::SIZE *  bodycount;
    Simtypes::v3 *    positions;
    Simtypes::v3 *    old_positions;
    Simtypes::v3 *    acceleration;
};

#endif
