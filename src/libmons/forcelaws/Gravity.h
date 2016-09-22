#ifndef _GRAVITY_H
#define _GRAVITY_H

#include <cmath>
#include <limits>

#include "../simulation/SimulationData.h"
#include "../util/PhysicalConstants.h"

// ToDo(robin): collision detection

class Gravity {
public:
    Gravity(SimulationData * data) noexcept;

    void apply(Simtypes::FLOAT simTime) noexcept;

private:
    void apply() noexcept;

    Simtypes::SIZE *  bodycount;
    Simtypes::FLOAT * mass;
    Simtypes::v3 *    positions;
    Simtypes::v3 *    accelerations;
};

#endif
