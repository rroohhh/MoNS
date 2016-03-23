#include <iostream>
#include <chrono>
#include <thread>

#include "util/DebugOutput.h"
#include "util/CommandLineOptions.h"
#include "util/cpptoml.h"
#include "simulation/Simulation.h"
#include "plot2D/GLPlot.h"

int main(int argc, char ** argv) {
	// parse the commandline options
	CommandLineOptions ops = CommandLineOptions(argc, argv);

	int time = 10;

	// build new simulation from configfile
	Simulation sim(ops.configFile());


	GLPlot * plot = nullptr;

	// only open gui if wanted
	if(ops.gui())
	    plot = new GLPlot();

	// buffer for plotting
	float * buffer = nullptr;

	// plotting handler
	if(ops.gui()) {
		sim.addStepListener([&](Simtypes::FLOAT simTime, Simtypes::SIZE bodycount, Simtypes::v3 * positions) {
				if(buffer == nullptr) {
					buffer = Platform::allocate<float>(bodycount);
				}

				if(std::fmod(simTime, time) == 0) {
					for (int i = 0; i < bodycount; i++) {
						buffer[2 * i] = positions[i].x;
						buffer[2 * i  + 1] = positions[i].y;
					}

					plot->plot(buffer, bodycount);
				}
			});
	}

	// print some information
	sim.addFinishListener([](Simtypes::SIZE bodycount, Simtypes::v3 * positions){
			std::cout << "Simulation finished" << std::endl;
			for (int i = 0; i < bodycount; i++) {
				std::cout << positions[i].x << "\t"
						  << positions[i].x << "\t"
						  << positions[i].x << std::endl;
			}
		});

	sim.start();

	sim.waitToEnd();
}
