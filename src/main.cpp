#include <chrono>
#include <iostream>
#include <thread>

#include "forcelaws/Gravity.h"
#include "integrators/Leapfrog.h"
#include "plot2D/GLPlot.h"
#include "simulation/Simulation.h"
#include "util/CommandLineOptions.h"
#include "util/DebugOutput.h"
#include "util/cpptoml.h"

int main(int argc, char ** argv) {
    // parse the commandline options
    CommandLineOptions ops = CommandLineOptions(argc, argv);

    int time = 10;

    // build new simulation from configfile
    Simulation<Gravity, Leapfrog> sim(ops.configFile());

    GLPlot * plot = nullptr;


    // buffer for plotting
    float * buffer = nullptr;

    // plotting handler
    if(ops.gui()) {
    	// only open gui if wanted
    	plot = new GLPlot();

        sim.addStepListener([&](Simtypes::FLOAT simTime,
                                Simtypes::SIZE  bodycount,
                                Simtypes::v3 *  positions) {
            if(buffer == nullptr) {
                buffer = Platform::allocate<float>(bodycount);
            }

            if(std::fmod(simTime, time) == 0) {
                for(Simtypes::SIZE i = 0; i < bodycount; i++) {
                    buffer[2 * i]     = positions[i].x;
                    buffer[2 * i + 1] = positions[i].y;
                }

                plot->plot(buffer, bodycount);
            }
        });
    }

    // print some information
    sim.addFinishListener(
        [](Simtypes::SIZE bodycount, Simtypes::v3 * positions) {
            std::cout << "Simulation finished" << std::endl;
            for(Simtypes::SIZE i = 0; i < bodycount; i++) {
                std::cout << positions[i].x << "\t" << positions[i].y << "\t"
                          << positions[i].z << std::endl;
            }
        });

    sim.start();

    sim.waitToEnd();
}
