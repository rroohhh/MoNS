#ifndef _GRAVITY_H
#define _GRAVITY_H

#include <cmath>
#include <limits>

#include "../platform/Platform.h"
#include "../simulation/SimulationConfig.h"
#include "../simulation/SimulationData.h"
#include "../util/PhysicalConstants.h"

// ToDo(robin): collision detection

class GravityBHut {
public:
    GravityBHut(SimulationData * data);

    void apply(Simtypes::FLOAT simTime);

private:
    void init();
    void apply();

    Simtypes::SIZE *  bodycount;
    Simtypes::FLOAT * mass;
    Simtypes::v3 *    positions;
    Simtypes::v3 *    accelerations;
};

#endif
