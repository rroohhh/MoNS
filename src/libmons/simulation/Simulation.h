#ifndef _SIMULATION_H
#define _SIMULATION_H

#include <csignal>
#include <string>
#include <thread>
#include <vector>

#include "Simtypes.h"
#include "SimulationConfig.h"
#include "SimulationData.h"

#include "../bench/TimedBlock.h"
#include "../forcelaws/Gravity.h"
#include "../integrators/Leapfrog.h"

template <typename ForceLaw, typename Integrator>
struct Simulation {
    // build a new simulation based on a simulationconfig
    inline Simulation(SimulationConfig * config) noexcept : m_config(config) {
        setup_signal_handler();

        m_data = new SimulationData(config);

        m_forcelaw   = new ForceLaw(m_data);
        m_integrator = new Integrator(m_data);
    }

    // parse the simulationconfig from a file
    inline Simulation(std::string filename) noexcept
        : Simulation(new SimulationConfig(filename)) {}

    inline int addStepListener(
        std::function<void(Simtypes::FLOAT, Simtypes::SIZE, Simtypes::v3 *)>
            listener) noexcept {
        TIMED_BLOCK(add_step_listener);
        m_stepListener->push_back(listener);

        return 0;
    }

    inline int addFinishListener(
        std::function<void(Simtypes::SIZE, Simtypes::v3 *)> listener) noexcept {
        TIMED_BLOCK(add_finish_listener);
        m_finishListener->push_back(listener);

        return 0;
    }

    inline int start() noexcept {
        TIMED_BLOCK(simulation_start);
        if(simulationThread != nullptr) {
            log::err("Simulation already started");
            std::abort();
        }

        // start simulation async in new thread
        simulationThread = new std::thread([&]() {
            // ToDo(robin): flexstep? -> not here, maybe in integrator module
            // calculate target timestep and in forcelaw use the calculated
            // timestep
            // ToDo(robin): find good method for flexsteps
            // ToDo(robin): are flexsteps actually needed?
            // or use force softening

            Simtypes::SIZE  totalSteps = (m_config->endTime / m_config->dt);
            Simtypes::FLOAT targetTime, reachedTime = 0;

            // run through all timesteps
            for(Simtypes::SIZE i = 0; i < totalSteps; i++) {
                TIMED_BLOCK(simulation_step);
                targetTime = m_config->dt * i;

                // basic flexstep support
                do {
                    m_forcelaw->apply(targetTime);
                    reachedTime = m_integrator->step(targetTime);
                } while(reachedTime < targetTime);

                for(auto listener : *m_stepListener) {
                    TIMED_BLOCK(simulation_step_listener);
                    listener(targetTime, m_data->bodycount, m_data->positions);
                }
            }

            for(auto listener : *m_finishListener) {
                TIMED_BLOCK(simulation_finish_listener);
                listener(m_data->bodycount, m_data->positions);
            }
        });

        return 0;
    }

    // join simulation thread to block execution until simulation ends
    inline int waitToEnd() noexcept {
        if(simulationThread == nullptr) start();

        simulationThread->join();
        return 0;
    }

    ~Simulation() { cleanup(); }

private:
    static void cleanup() { TimedBlock::output_timings(); }

    void (*signal_handler)(int) = [](int i) noexcept {
        log::err("Interrupt {} caught", i);
        cleanup();
        std::exit(EXIT_SUCCESS);
    };

    void setup_signal_handler() {
        struct sigaction act;
        act.sa_handler = signal_handler;
        sigemptyset(&act.sa_mask);
        act.sa_flags = 0;
        sigaction(SIGINT, &act, 0);
    }

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
