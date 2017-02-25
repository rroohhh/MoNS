#ifndef __SIMULATIONDATA_H_
#define __SIMULATIONDATA_H_

#include "platform.h"
#include "simulation_config.h"
#include "types.h"

// container class for (nearly) all data used by a simulation
namespace mons {
    struct simulation_data {
        v3 * positions;
        f *  mass; // Merge mass + positon for better cache usage?
        v3 * acceleration;
        v3 * old_positions;
        v3 * velocities;
        f *  dt;
        size bodycount;

        simulation_data()                              = delete;
        simulation_data(const simulation_data & other) = delete;
        simulation_data(simulation_data && other)      = delete;
        simulation_data & operator=(const simulation_data & other) = delete;
        simulation_data & operator=(simulation_data && other) = delete;

        simulation_data(simulation_config * config) noexcept {
            this->bodycount = config->bodycount;

            positions     = platform::allocateInit<v3>(bodycount);
            old_positions = platform::allocateInit<v3>(bodycount);
            acceleration  = platform::allocateInit<v3>(bodycount);
            velocities    = platform::allocateInit<v3>(bodycount);
            mass          = platform::allocateInit<f>(bodycount);
            dt            = platform::allocateInit<f>(bodycount);

            // copy the known data to it's target location
            platform::copy(config->masses, mass, bodycount);
            platform::copy(config->positions, positions, bodycount);
            platform::copy(config->velocities, velocities, bodycount);

            // init dt with the global config
            for(size i = 0; i < bodycount; i++) { dt[i] = config->dt; }
        }

        ~simulation_data() noexcept {
            platform::deallocate(positions);
            platform::deallocate(old_positions);
            platform::deallocate(acceleration);
            platform::deallocate(mass);
            platform::deallocate(velocities);
            platform::deallocate(dt);
        };
    };
}

#endif
