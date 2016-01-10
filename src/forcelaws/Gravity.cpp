#include "Gravity.h"

// Todo: literally everything

Gravity::Gravity(SimulationData * data) {
	bodycount = &data->bodycount;
	mass = data->mass;
	positions = data->positions;
	accelerations = data->acceleration;

	for(Simtypes::SIZE i = 0; i < *bodycount; i++) {
		mass[i] = mass[i] *	PhysicalConstants::gamma;
	}

	apply();
}

void Gravity::apply() {
	this->apply(std::numeric_limits<Simtypes::FLOAT>::max());
}

void Gravity::apply(Simtypes::FLOAT simTime) {
	auto count = *bodycount;

	Simtypes::FLOAT dx, dy, dz, dist, p;

	for(Simtypes::SIZE i = 0; i < count; i++) {
		for(Simtypes::SIZE j = i + 1; j < count; j++) {
			dx = positions[i].x - positions[j].x;
			dy = positions[i].y - positions[j].y;
			dz = positions[i].z - positions[j].z;
			dist = sqrt(dx * dx + dy * dy + dz * dz);
			dist = dist * dist * dist;

			p = dx / dist;
			accelerations[i].x -= p * mass[j];
			accelerations[j].x += p * mass[i];

			p = dy / dist;
			accelerations[i].y -= p * mass[j];
			accelerations[j].y += p * mass[i];

			p = dz / dist;
			accelerations[i].z -= p * mass[j];
			accelerations[j].z += p * mass[i];
		}
	}
}

Gravity::~Gravity() {

}
