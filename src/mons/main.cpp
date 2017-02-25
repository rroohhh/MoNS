#include "bench.h"
#include "cli_options.h"
#include "forcelaws/gravity_naive.h"
#include "glplot.h"
#include "integrators/leapfrog.h"
#include "platform.h"
#include "repl.h"
#include "simulation/simulation.h"
#include "sshd.h"

using namespace mons;
using Sim = simulation<gravity_naive, leapfrog>;

int main(int argc, char ** argv) {
    TIMED_BLOCK(setup);
    FILE * logfile = fopen("mons.log", "aw");

    // redirect log output to( file
    /* log::log_outputs.erase("stdout"); */
    /* log::log_outputs["logfile"] = logfile; */

    // parse the commandline options
    cli_options ops = cli_options(argc, argv);

    // build new simulation from configfile
    Sim sim(ops.config_file());

    repl<stdio, Sim> r({}, {&sim}, false);

    if(ops.ssh()) {
        thread([&]() {
            sshd listener("0.0.0.0", 8000);
            while(true) {
                auto io = listener.accept();
                repl<sshio, Sim>(io, {&sim});
            }
        }).detach();
    }

    // plotting handler
    if(ops.gui()) {
        // only open gui if wanted
        auto * plot = new glplot();

        //  buffer for plotting
        float * buffer = nullptr;

        int delta_time = 10;

        sim.add_step_listener([&](f sim_time, size bodycount, v3 * positions) {
            TIMED_BLOCK(plot);
            if(buffer == nullptr) {
                buffer = platform::allocate<float>(2 * bodycount);
            }

            /* if(fmod(sim_time, delta_time) == 0) { */
                for(size i = 0; i < bodycount; i++) {
                    buffer[2 * i]     = positions[i].pos[0];
                    buffer[2 * i + 1] = positions[i].pos[1];
                }

                plot->plot(buffer, bodycount);
            /* } */
        });
    }

    // print some information
    sim.add_finish_listener([](size bodycount, v3 * positions) {
        log::info("simulation finished");
        for(size i = 0; i < bodycount; i++) {
            log::info("{}\t{}\t{}", positions[i].pos[0], positions[i].pos[1],
                      positions[i].pos[2]);
        }
    });

    END_BLOCK(setup)

    sim.wait_to_end();
}
