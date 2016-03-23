#include "Leapfrog.h"

Leapfrog::Leapfrog(SimulationData * data) {
	bodycount = &data->bodycount;
	positions = data->positions;
	old_positions = data->old_positions;
	acceleration = data->acceleration;
	dt = data->dt;

	init(data->velocities);
}


// ToDo(robin): SIMD
// calculate first step with euler's method, because no old position is known
void Leapfrog::init(Simtypes::v3 * velocities) {
	Platform::copy(positions, old_positions, *bodycount);

	Simtypes::FLOAT this_dt;
	Simtypes::FLOAT dt2;

	for(Simtypes::SIZE i = 0; i < *bodycount; i++) {
		this_dt = dt[i];
		dt2 = 0.5 * this_dt * this_dt;

		// one iteration of euler's method
		positions[i].x = old_positions[i].x + velocities[i].x * this_dt + acceleration[i].x * dt2;
		positions[i].y = old_positions[i].y + velocities[i].y * this_dt + acceleration[i].y * dt2;
		positions[i].z = old_positions[i].z + velocities[i].z * this_dt + acceleration[i].z * dt2;
	}
}

// ToDo(robin): SIMD
// ToDo(robin): Flexsteps
// now use actual leapfrog method
Simtypes::FLOAT Leapfrog::step(Simtypes::FLOAT simTime) {
	Simtypes::FLOAT dt2;
	Simtypes::v3 tmp_position;

	for(Simtypes::SIZE i = 0; i < *bodycount; i++) {
		dt2 = dt[i] * dt[i];

		tmp_position.x = positions[i].x;
		tmp_position.y = positions[i].y;
		tmp_position.z = positions[i].z;

		positions[i].x = 2 * positions[i].x - old_positions[i].x + acceleration[i].x * dt2;
		positions[i].y = 2 * positions[i].y - old_positions[i].y + acceleration[i].y * dt2;
		positions[i].z = 2 * positions[i].z - old_positions[i].z + acceleration[i].z * dt2;

		old_positions[i].x = tmp_position.x;
		old_positions[i].y = tmp_position.y;
		old_positions[i].z = tmp_position.z;

		// reset acceleration to zero for the next step
		acceleration[i].x = 0;
		acceleration[i].y = 0;
		acceleration[i].z = 0;
	}

	return simTime;
}

Leapfrog::~Leapfrog() {}
