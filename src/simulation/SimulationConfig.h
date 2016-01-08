#ifndef _SIMULATIONCONFIG_H
#define _SIMULATIONCONFIG_H

#include "Simtypes.h"

class SimulationConfig {
public:
	SimulationConfig(Simtypes::SIZE bodycount, Simtypes::FLOAT endTime, Simtypes::FLOAT dt,
					 Simtypes::v3 * positions, Simtypes::v3 * velocities) :
		bodycount(bodycount), endTime(endTime), dt(dt), positions(positions),
		velocities(velocities)  {}

	Simtypes::SIZE bodycount;
	Simtypes::FLOAT endTime;
	Simtypes::FLOAT dt;
	Simtypes::v3 * positions;
	Simtypes::v3 * velocities;
};

#endif
