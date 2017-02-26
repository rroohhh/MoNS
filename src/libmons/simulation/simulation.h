#ifndef __SIMULATION_H_
#define __SIMULATION_H_

#include <csignal>

#include "simulation_config.h"
#include "simulation_data.h"
#include "types.h"

#include "bench.h"

namespace mons {
    template <typename ForceLaw, typename Integrator>
    struct simulation {
        using step_listener_func   = function<void(f, size, f *, f *, f *)>;
        using finish_listener_func = function<void(size, f *, f *, f *)>;

        // build a new simulation based on a simulationconfig
        inline simulation(simulation_config * config) noexcept
            : config(config) {
            setup_signal_handler();

            data = new simulation_data(config);

            forcelaw   = new ForceLaw(data);
            integrator = new Integrator(data);
        }

        // parse the simulationconfig from a file
        inline simulation(string filename) noexcept
            : simulation(new simulation_config(filename)) {}

        simulation()                              = delete;
        simulation(const simulation_data & other) = delete;
        simulation(simulation_data && other)      = delete;
        simulation & operator=(const simulation_data & other) = delete;
        simulation & operator=(simulation_data && other) = delete;

        inline int
        add_step_listener(const step_listener_func & listener) noexcept {
            TIMED_BLOCK(add_step_listener);
            step_listener.push_back(listener);

            return 0;
        }

        inline int
        add_finish_listener(const finish_listener_func & listener) noexcept {
            TIMED_BLOCK(add_finish_listener);
            finish_listener.push_back(listener);

            return 0;
        }

        inline int start() noexcept {
            TIMED_BLOCK(simulation_start);
            if(simulation_thread != nullptr) {
                log::err("Simulation already started");
                abort();
            }

            // start simulation async in new thread
            simulation_thread = new thread([&]() {
                // ToDo(robin): flexstep? -> not here, maybe in integrator
                // module
                // calculate target timestep and in forcelaw use the calculated
                // timestep
                // ToDo(robin): find good method for flexsteps
                // ToDo(robin): are flexsteps actually needed?
                // or use force softening

                start_time = high_resolution_clock::now();

                size total_steps = (config->endTime / config->dt);
                f    target_time, reached_time = 0;

                // run through all timesteps
                for(current_steps = 0; current_steps < total_steps;
                    current_steps++) {
                    TIMED_BLOCK(simulation_step);
                    target_time = config->dt * current_steps;

                    // basic flexstep support
                    do {
                        TIMED_BLOCK(forcelaw);
                        forcelaw->apply(target_time);
                        END_BLOCK(forcelaw);

                        TIMED_BLOCK(integrator);
                        reached_time = integrator->step(target_time);
                        END_BLOCK(integrator);
                    } while(reached_time < target_time);

                    for(const auto & listener : step_listener) {
                        TIMED_BLOCK(simulation_step_listener);
                        listener(target_time, data->bodycount, data->px, data->py, data->pz);
                    }
                }

                for(const auto & listener : finish_listener) {
                    TIMED_BLOCK(simulation_finish_listener);
                    listener(data->bodycount, data->px, data->py, data->pz);
                }
            });

            return 0;
        }

        inline size steps() { return current_steps; }

        inline simulation_config * sim_config() { return config; }

        inline high_resolution_clock::time_point starting_time() {
            return start_time;
        }

        // join simulation thread to block execution until simulation ends
        inline i32 wait_to_end() noexcept {
            if(simulation_thread == nullptr) start();

            simulation_thread->join();
            return 0;
        }

        ~simulation() { cleanup(); }

    private:
        static void cleanup() { timed_block::output_timings(); }

        void (*signal_handler)(int) = [](int i) noexcept {
            log::err("Interrupt {} caught", i);
            cleanup();
            exit(EXIT_SUCCESS);
        };

        void setup_signal_handler() {
            struct sigaction act;
            act.sa_handler = signal_handler;
            sigemptyset(&act.sa_mask);
            act.sa_flags = 0;
            sigaction(SIGINT, &act, nullptr);
        }

        simulation_data *   data;
        simulation_config * config;

        ForceLaw *   forcelaw; // also does Collision detection
        Integrator * integrator;

        high_resolution_clock::time_point start_time;

        size current_steps = 0;

        thread * simulation_thread = nullptr;

        vector<step_listener_func>   step_listener;
        vector<finish_listener_func> finish_listener;
    };
}

#endif
