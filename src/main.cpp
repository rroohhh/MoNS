#include <iostream>
#include <chrono>
#include <thread>
#include "util/DebugOutput.h"
#include "util/cpptoml.h"
#include "simulation/Simulation.h"
#include "plot2D/GLPlot.h"

int main(int argc, char ** argv) {
	Simulation sim("config.toml");
	GLPlot plot;
	double tmp;

	float * buffer = nullptr;

	sim.addStepListener([&plot, &tmp, &buffer, &argv](Simtypes::FLOAT simTime, Simtypes::SIZE bodycount, Simtypes::v3 * positions) {
			if(buffer == nullptr) {
				buffer = Platform::allocate<float>(bodycount);
			}

			if(std::fmod(simTime, std::stoi(std::string(argv[1]))) == 0) {
				for (int i = 0; i < bodycount; i++) {
					buffer[2 * i] = positions[i].x;
					buffer[2 * i  + 1] = positions[i].y;
				}

				plot.plot(buffer, bodycount);
			}
		});

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
