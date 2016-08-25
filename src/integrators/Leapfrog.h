#ifndef _LEAPFROG_H
#define _LEAPFROG_H

#include "../platform/Platform.h"
#include "../simulation/SimulationConfig.h"
#include "../simulation/SimulationData.h"

class Leapfrog {
public:
    Leapfrog(SimulationData * data);

    Simtypes::FLOAT step(Simtypes::FLOAT simTime);

    ~Leapfrog();

private:
    void init(Simtypes::v3 * velocities);

    Simtypes::FLOAT * dt;
    Simtypes::SIZE *  bodycount;
    Simtypes::v3 *    positions;
    Simtypes::v3 *    old_positions;
    Simtypes::v3 *    acceleration;
};

#endif
