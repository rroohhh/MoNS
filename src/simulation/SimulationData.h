#ifndef _SIMULATIONDATA_H
#define _SIMULATIONDATA_H

#include "../platform/Platform.h"
#include "Simtypes.h"
#include "SimulationConfig.h"

// container class for (nearly) all data used by a simulation

class SimulationData {
public:
	Simtypes::v3 * positions;
	Simtypes::FLOAT * mass;      // Merge mass and position into one array for better cache usage?
	Simtypes::v3 * acceleration;
	Simtypes::v3 * old_positions;
	Simtypes::v3 * velocities;
	Simtypes::FLOAT * dt;
	Simtypes::SIZE bodycount;

	SimulationData(SimulationConfig * config) {
		Simtypes::SIZE bodycount = config->bodycount;

		positions = Platform::allocate<Simtypes::v3>(bodycount);
	    old_positions = Platform::allocate<Simtypes::v3>(bodycount);
	    acceleration = Platform::allocate<Simtypes::v3>(bodycount);
	    velocities = Platform::allocate<Simtypes::v3>(bodycount);
		mass = Platform::allocate<Simtypes::FLOAT>(bodycount);
		dt = Platform::allocate<Simtypes::FLOAT>(bodycount);

		this->bodycount = bodycount;

		// copy the known data to it's target location
		Platform::copy(config->masses, mass, bodycount);
		Platform::copy(config->positions, positions, bodycount);
		Platform::copy(config->velocities, velocities, bodycount);

		// init dt with the global config
		for(Simtypes::SIZE i = 0; i < bodycount; i++) {
			dt[i] = config->dt;
		}
	}

	~SimulationData() {
		Platform::deallocate(positions);
		Platform::deallocate(old_positions);
		Platform::deallocate(acceleration);
		Platform::deallocate(mass);
		Platform::deallocate(velocities);
		Platform::deallocate(dt);
	};
};

#endif
