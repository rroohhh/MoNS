#include <chrono>
#include <iostream>
#include <thread>

#include "GLPlot.h"
#include "bench/TimedBlock.h"
#include "forcelaws/Gravity.h"
#include "integrators/Leapfrog.h"
#include "platform/Platform.h"
#include "repl.h"
#include "simulation/Simulation.h"
#include "sshd.h"
#include "util/CommandLineOptions.h"

using namespace std::chrono_literals;
using namespace io;
using Sim = Simulation<Gravity, Leapfrog>;

int main(int argc, char ** argv) {

    TIMED_BLOCK(setup);
    /* FILE * logfile = fopen("mons.log", "aw"); */

    // redirect log output to file
    /* io::log::log_outputs.erase("stdout"); */
    /* io::log::log_outputs["logfile"] = logfile; */

    /* repl<stdio> r({}, CommandHandler()); */
    // parse the commandline options
    CommandLineOptions ops = CommandLineOptions(argc, argv);

    // build new simulation from configfile
    Sim sim(ops.configFile());

    std::thread ssh_thread([&]() {
        sshd listener("0.0.0.0", 8000);
        while(true) {
            auto io = listener.accept();
            repl<sshio, Sim>(io, CommandHandler<Sim>(&sim));
        }
    });

    // plotting handler
    if(ops.gui()) {
        // only open gui if wanted
        auto * plot = new GLPlot();

        //  buffer for plotting
        float * buffer = nullptr;

        int delta_time = 10;

        sim.addStepListener([&](Simtypes::FLOAT simTime,
                                Simtypes::SIZE  bodycount,
                                Simtypes::v3 *  positions) {
            TIMED_BLOCK(plot);
            if(buffer == nullptr) {
                buffer = Platform::allocate<float>(2 * bodycount);
            }

            if(std::fmod(simTime, delta_time) == 0) {
                for(Simtypes::SIZE i = 0; i < bodycount; i++) {
                    buffer[2 * i]     = positions[i].pos[0];
                    buffer[2 * i + 1] = positions[i].pos[1];
                }

                plot->plot(buffer, bodycount);
            }
        });
    }

    // print some information
    sim.addFinishListener(
        [](Simtypes::SIZE bodycount, Simtypes::v3 * positions) {
            log::info("Simulation finished");
            for(Simtypes::SIZE i = 0; i < bodycount; i++) {
                log::info("{}\t{}\t{}", positions[i].pos[0],
                          positions[i].pos[1], positions[i].pos[2]);
            }
        });

    END_BLOCK(setup)

    sim.waitToEnd();
}
