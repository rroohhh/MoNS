#ifndef _SIMULATION_H
#define _SIMULATION_H

#include <thread>
#include <vector>

#include "Simtypes.h"
#include "SimulationConfig.h"
#include "SimulationData.h"

#include "../forcelaws/ForceLaw.h"
#include "../integrators/Integrator.h"

class Simulation {
public:
	Simulation(SimulationConfig * config) {
		m_config = config;

		m_data = new SimulationData(config);

		m_forcelaw = new Gravity(config, m_data);
		m_integrator = new Leapfrog(config, m_data);
	}

	int addStepListener(std::function<void(Simtypes::FLOAT, Simtypes::SIZE, Simtypes::v3 *)> listener) {
		m_stepListener->push_back(listener);

		return 0;
	}

	int addFinishListener(std::function<void(Simtypes::SIZE, Simtypes::v3 *)> listener) {
		m_finishListener->push_back(listener);

		return 0;
	}

	int start() {
		new std::thread([=] () {
				// ToDo: flexstep? -> not here, maybe in integrator module calculate target timestep and in forcelaw use the calculated timestep
				// or use force softening

				// ToDo call listeners

				int totalSteps = (m_config->endtime / m_config->dt);
				Simtypes::FLOAT targetTime, reachedTime = 0;

				for (int i = 0; i < totalSteps; i++) {
					targetTime = m_config->dt * i;

					do {
						m_forcelaw->apply(simTime);
						reachedTime = m_integrator->step(simTime);
					} while(reachedTime < targetTime);

					for(auto listener : m_stepListener) {
						listener(targetTime, m_data.bodycount, m_data.positions);
					}
				}

				for(auto listener : m_finishListener) {
					listener(m_data.bodycount, m_data.positons);
				}
			})->detach();

		return 0;
	}

	~Simulation();
private:
	SimulationData * m_data;
	SimulationConfig * m_config;

	ForceLaw * m_forcelaw; // Also does Collision detection
    Integrator * m_integrator;

	std::vector<std::function<void(Simtypes::FLOAT, Simtypes::SIZE, Simtypes::v3 *)>> * m_stepListener = new std::vector<void(Simtypes::FLOAT, Simtypes::SIZE, Simtypes::v3 *)>();
	std::vector<std::function<void(Simtypes::SIZE, Simtypes::v3 *)>> * m_finishListener = new std::vector<void(Simtypes::SIZE, Simtypes::v3 *)>();
};

#endif
