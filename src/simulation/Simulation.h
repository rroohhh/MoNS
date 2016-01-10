#ifndef _SIMULATION_H
#define _SIMULATION_H

#include <thread>
#include <vector>
#include <string>

#include "Simtypes.h"
#include "SimulationConfig.h"
#include "SimulationData.h"

#include "../forcelaws/ForceLaw.h"
#include "../forcelaws/Gravity.h"
#include "../integrators/Leapfrog.h"
#include "../integrators/Integrator.h"

class Simulation {
public:
	Simulation(SimulationConfig * config) {
		m_config = config;

		m_data = new SimulationData(config);

		m_forcelaw = new Gravity(m_data);
		m_integrator = new Leapfrog(m_data);
	}

	Simulation(std::string filename) : Simulation(new SimulationConfig(filename)) {}

	int addStepListener(std::function<void(Simtypes::FLOAT, Simtypes::SIZE, Simtypes::v3 *)> listener) {
		m_stepListener->push_back(listener);

		return 0;
	}

	int addFinishListener(std::function<void(Simtypes::SIZE, Simtypes::v3 *)> listener) {
		m_finishListener->push_back(listener);

		return 0;
	}

	int start() {
		if(simulationThread != nullptr) {
			std::cerr << "Simulation already started" << std::endl;
			std::abort();
		}

		simulationThread = new std::thread([=] () {
				// ToDo: flexstep? -> not here, maybe in integrator module calculate target timestep and in forcelaw use the calculated timestep
				// or use force softening

				// ToDo call listeners

				Simtypes::SIZE totalSteps = (m_config->endTime / m_config->dt);
				Simtypes::FLOAT targetTime, reachedTime = 0;

				for (int i = 0; i < totalSteps; i++) {
					targetTime = m_config->dt * i;

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

		//simulationThread->detach();

		return 0;
	}

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
	SimulationData * m_data;
	SimulationConfig * m_config;

	ForceLaw * m_forcelaw; // Also does Collision detection
    Integrator * m_integrator;

	std::thread * simulationThread = nullptr;

	std::vector<std::function<void(Simtypes::FLOAT, Simtypes::SIZE, Simtypes::v3 *)>> * m_stepListener = new std::vector<std::function<void(Simtypes::FLOAT, Simtypes::SIZE, Simtypes::v3 *)>>();
	std::vector<std::function<void(Simtypes::SIZE, Simtypes::v3 *)>> * m_finishListener = new std::vector<std::function<void(Simtypes::SIZE, Simtypes::v3 *)>>();
};

#endif
