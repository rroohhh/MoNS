#ifndef _SIMULATION_H
#define _SIMULATION_H

#include <string>
#include <thread>
#include <vector>

#include "Simtypes.h"
#include "SimulationConfig.h"
#include "SimulationData.h"

#include "../forcelaws/Gravity.h"
#include "../integrators/Leapfrog.h"

template <typename ForceLaw, typename Integrator>
struct Simulation {
    // build a new simulation based on a simulationconfig
    Simulation(SimulationConfig * config) {
        m_config = config;

        m_data = new SimulationData(config);

        m_forcelaw   = new ForceLaw(m_data);
        m_integrator = new Integrator(m_data);
    }

    // parse the simulationconfig from a file
    Simulation(std::string filename)
        : Simulation(new SimulationConfig(filename)) {}

    int addStepListener(
        std::function<void(Simtypes::FLOAT, Simtypes::SIZE, Simtypes::v3 *)>
            listener) {
        m_stepListener->push_back(listener);

        return 0;
    }

    int addFinishListener(
        std::function<void(Simtypes::SIZE, Simtypes::v3 *)> listener) {
        m_finishListener->push_back(listener);

        return 0;
    }

    int start() {
        if(simulationThread != nullptr) {
            std::cerr << "Simulation already started" << std::endl;
            std::abort();
        }

        // start simulation async in new thread
        simulationThread = new std::thread([=]() {
            // ToDo(robin): flexstep? -> not here, maybe in integrator module
            // calculate target timestep and in forcelaw use the calculated
            // timestep
            // ToDo(robin): find good method for flexsteps
            // ToDo(robin): are flexsteps actually needed?
            // or use force softening

            Simtypes::SIZE  totalSteps = (m_config->endTime / m_config->dt);
            Simtypes::FLOAT targetTime, reachedTime = 0;

            // run through all timesteps
            for(int i = 0; i < totalSteps; i++) {
                targetTime = m_config->dt * i;

                // basic flexstep support
                do {
                    m_forcelaw->apply(targetTime);
                    reachedTime = m_integrator->step(targetTime);
                } while(reachedTime < targetTime);

                for(auto listener : *m_stepListener) {
                    listener(targetTime, m_data->bodycount, m_data->positions);
                }
            }

            for(auto listener : *m_finishListener) {
                listener(m_data->bodycount, m_data->positions);
            }
        });

        return 0;
    }

    // join simulation thread to block execution until simulation ends
    int waitToEnd() {
        if(simulationThread == nullptr) {
            std::cerr << "no simulation started" << std::endl;
            std::abort();
        } else {
            simulationThread->join();
        }

        return 0;
    }

    ~Simulation() {}

private:
    SimulationData *   m_data;
    SimulationConfig * m_config;

    ForceLaw *   m_forcelaw; // also does Collision detection
    Integrator * m_integrator;

    std::thread * simulationThread = nullptr;

    std::vector<std::function<void(Simtypes::FLOAT, Simtypes::SIZE,
                                   Simtypes::v3 *)>> * m_stepListener =
        new std::vector<std::function<void(Simtypes::FLOAT, Simtypes::SIZE,
                                           Simtypes::v3 *)>>();
    std::vector<std::function<void(Simtypes::SIZE,
                                   Simtypes::v3 *)>> * m_finishListener =
        new std::vector<std::function<void(Simtypes::SIZE, Simtypes::v3 *)>>();
};

#endif
