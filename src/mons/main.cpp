#include <chrono>
#include <iostream>
#include <thread>

#include <readline/history.h>
#include <readline/readline.h>

#include "GLPlot.h"
#include "bench/TimedBlock.h"
#include "forcelaws/Gravity.h"
#include "integrators/Leapfrog.h"
#include "platform/Platform.h"
#include "repl.h"
#include "simulation/Simulation.h"
#include "ssh_file.h"
#include "sshd.h"
#include "util/CommandLineOptions.h"

using namespace io;

int main(int argc, char ** argv) {
    TIMED_BLOCK(setup);
    // std::thread([]() {
    //     sshd listener("0.0.0.0", 8000);
    //     while(true) {
    //         auto     chan = listener.accept();
    //         ssh_file sfile(&chan);
    //         repl(sfile.infile(), sfile.outfile(), CommandHandler());
    //     }
    // }).detach();

	repl r(stdin, stdout, CommandHandler());

    // parse the commandline options
    CommandLineOptions ops = CommandLineOptions(argc, argv);

    // build new simulation from configfile
    Simulation<Gravity, Leapfrog> sim(ops.configFile());

    // plotting handler
    if(ops.gui()) {
        // only open gui if wanted
        GLPlot * plot = new GLPlot();

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
