#ifndef _SIMULATIONDATA_H
#define _SIMULATIONDATA_H

#include "../platform/Platform.h"
#include "Simtypes.h"
#include "SimulationConfig.h"

// container class for (nearly) all data used by a simulation

struct SimulationData {
    Simtypes::v3 *    positions;
    Simtypes::FLOAT * mass; // Merge mass + positon for better cache usage?
    Simtypes::v3 *    acceleration;
    Simtypes::v3 *    old_positions;
    Simtypes::v3 *    velocities;
    Simtypes::FLOAT * dt;
    Simtypes::SIZE    bodycount;

	SimulationData() = delete;
	SimulationData(const SimulationData & other) = delete;
	SimulationData(SimulationData && other) = delete;
	SimulationData & operator=(const SimulationData & other) = delete;
	SimulationData & operator=(SimulationData && other) = delete;

    SimulationData(SimulationConfig * config) noexcept {
        this->bodycount = config->bodycount;

        positions     = Platform::allocateInit<Simtypes::v3>(bodycount);
        old_positions = Platform::allocateInit<Simtypes::v3>(bodycount);
        acceleration  = Platform::allocateInit<Simtypes::v3>(bodycount);
        velocities    = Platform::allocateInit<Simtypes::v3>(bodycount);
        mass          = Platform::allocateInit<Simtypes::FLOAT>(bodycount);
        dt            = Platform::allocateInit<Simtypes::FLOAT>(bodycount);

        // copy the known data to it's target location
        Platform::copy(config->masses, mass, bodycount);
        Platform::copy(config->positions, positions, bodycount);
        Platform::copy(config->velocities, velocities, bodycount);

        // init dt with the global config
        for(Simtypes::SIZE i = 0; i < bodycount; i++) { dt[i] = config->dt; }
    }

    ~SimulationData() noexcept {
        Platform::deallocate(positions);
        Platform::deallocate(old_positions);
        Platform::deallocate(acceleration);
        Platform::deallocate(mass);
        Platform::deallocate(velocities);
        Platform::deallocate(dt);
    };
};

#endif
