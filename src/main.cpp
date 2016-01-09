#include <iostream>
#include "util/DebugOutput.h"
#include "util/cpptoml.h"
#include "simulation/Simulation.h"

int main() {
	DEBUGOUTPUT("Hello" << 5 << std::endl);
    std::cout << "Hello, World!" << std::endl;
	Simulation sim("config.toml");
}
