#include "Gravity.h"

Gravity::Gravity(SimulationData * data) noexcept {
    bodycount     = &data->bodycount;
    mass          = data->mass;
    positions     = data->positions;
    accelerations = data->acceleration;

    // premultiply all masses with gamma, saves some mul's later on
    for(Simtypes::SIZE i = 0; i < *bodycount; i++) {
        mass[i] = mass[i] * PhysicalConstants::gamma;
    }

    // apply one time to init the values for the old position
    apply();
}

void Gravity::apply() noexcept {
    this->apply(std::numeric_limits<Simtypes::FLOAT>::max());
}

// ToDo(robin): build support for flexsteps -> simTime
// ToDo(robin): SIMD
void Gravity::apply(Simtypes::FLOAT simTime) noexcept {
    auto count = *bodycount;

    Simtypes::FLOAT dx, dy, dz, dist, p;

    // naive force summation (uses counterforce force equality)
    for(Simtypes::SIZE i = 0; i < count; i++) {
        for(Simtypes::SIZE j = i + 1; j < count; j++) {
            // calculate distance
            dx   = positions[i].pos[0] - positions[j].pos[0];
            dy   = positions[i].pos[1] - positions[j].pos[1];
            dz   = positions[i].pos[2] - positions[j].pos[2];
            dist = sqrt(dx * dx + dy * dy + dz * dz);
            dist = dist * dist * dist;

            // add the respective force components (as acceleration) to the
            // total acceleration
            p = dx / dist;
            accelerations[i].pos[0] -= p * mass[j];
            accelerations[j].pos[0] += p * mass[i];

            p = dy / dist;
            accelerations[i].pos[1] -= p * mass[j];
            accelerations[j].pos[1] += p * mass[i];

            p = dz / dist;
            accelerations[i].pos[2] -= p * mass[j];
            accelerations[j].pos[2] += p * mass[i];
        }
    }
}
