#ifndef _GRAVITY_H
#define _GRAVITY_H

#include <limits>
#include <cmath>

#include "ForceLaw.h"
#include "../simulation/SimulationData.h"
#include "../simulation/SimulationConfig.h"
#include "../platform/Platform.h"
#include "../util/PhysicalConstants.h"

class Gravity : public ForceLaw {
public:
	Gravity(SimulationData * data);

    void apply(Simtypes::FLOAT simTime);

	~Gravity();

private:
	void init();
	void apply();

	Simtypes::SIZE * bodycount;
	Simtypes::FLOAT * mass;
	Simtypes::v3 * positions;
	Simtypes::v3 * accelerations;
};

#endif
