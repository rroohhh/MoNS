#ifndef __SIMULATIONDATA_H_
#define __SIMULATIONDATA_H_

#include "platform.h"
#include "simulation_config.h"
#include "types.h"

// container class for (nearly) all data used by a simulation
namespace mons {
    struct simulation_data {
        f *  px;
        f *  py;
        f *  pz;
        f *  mass; // Merge mass + positon for better cache usage?
        f *  ax;
        f *  ay;
        f *  az;
        f *  opx;
        f *  opy;
        f *  opz;
        f *  vx;
        f *  vy;
        f *  vz;
        f *  dt;
        size bodycount;

        simulation_data()                              = delete;
        simulation_data(const simulation_data & other) = delete;
        simulation_data(simulation_data && other)      = delete;
        simulation_data & operator=(const simulation_data & other) = delete;
        simulation_data & operator=(simulation_data && other) = delete;

        simulation_data(simulation_config * config) noexcept {
            this->bodycount = config->bodycount;

            px     = platform::allocateInit<f>(bodycount);
            py     = platform::allocateInit<f>(bodycount);
            pz     = platform::allocateInit<f>(bodycount);
            opx = platform::allocateInit<f>(bodycount);
            opy = platform::allocateInit<f>(bodycount);
            opz = platform::allocateInit<f>(bodycount);
            ax  = platform::allocateInit<f>(bodycount);
            ay  = platform::allocateInit<f>(bodycount);
            az  = platform::allocateInit<f>(bodycount);
            vx    = platform::allocateInit<f>(bodycount);
            vy    = platform::allocateInit<f>(bodycount);
            vz    = platform::allocateInit<f>(bodycount);
            mass          = platform::allocateInit<f>(bodycount);
            dt            = platform::allocateInit<f>(bodycount);

            // copy the known data to it's target location
            platform::copy(config->masses, mass, bodycount);

			for(size i = 0; i < bodycount; i++) {
				px[i] = config->positions[i].pos[0];
				py[i] = config->positions[i].pos[1];
				pz[i] = config->positions[i].pos[2];
				vx[i] = config->velocities[i].pos[0];
				vy[i] = config->velocities[i].pos[1];
				vz[i] = config->velocities[i].pos[2];
			}

            /* platform::copy((f *) config->positions, positions, 3 * bodycount); */
            /* platform::copy((f *) config->velocities, velocities, 3 * bodycount); */

            // init dt with the global config
            for(size i = 0; i < bodycount; i++) { dt[i] = config->dt; }
        }

        ~simulation_data() noexcept {
            platform::deallocate(px);
            platform::deallocate(py);
            platform::deallocate(pz);
            platform::deallocate(opx);
            platform::deallocate(opy);
            platform::deallocate(opz);
            platform::deallocate(ax);
            platform::deallocate(ay);
            platform::deallocate(az);
            platform::deallocate(mass);
            platform::deallocate(vx);
            platform::deallocate(vy);
            platform::deallocate(vz);
            platform::deallocate(dt);
        };
    };
}

#endif
