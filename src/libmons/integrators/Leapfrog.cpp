#include "Leapfrog.h"

Leapfrog::Leapfrog(SimulationData * data) noexcept {
    bodycount     = &data->bodycount;
    positions     = data->positions;
    old_positions = data->old_positions;
    acceleration  = data->acceleration;
    dt            = data->dt;

    init(data->velocities);
}

// ToDo(robin): SIMD
// calculate first step with euler's method, because no old position is known
inline void Leapfrog::init(Simtypes::v3 * velocities) noexcept {
    Platform::copy(positions, old_positions, *bodycount);

    Simtypes::FLOAT this_dt;
    Simtypes::FLOAT dt2;

    for(Simtypes::SIZE i = 0; i < *bodycount; i++) {
        this_dt = dt[i];
        dt2     = 0.5 * this_dt * this_dt;

        // one iteration of euler's method
        positions[i].pos[0] = old_positions[i].pos[0] +
                              velocities[i].pos[0] * this_dt +
                              acceleration[i].pos[0] * dt2;
        positions[i].pos[1] = old_positions[i].pos[1] +
                              velocities[i].pos[1] * this_dt +
                              acceleration[i].pos[1] * dt2;
        positions[i].pos[2] = old_positions[i].pos[2] +
                              velocities[i].pos[2] * this_dt +
                              acceleration[i].pos[2] * dt2;
    }
}

// ToDo(robin): SIMD
// ToDo(robin): Flexsteps
// now use actual leapfrog method
Simtypes::FLOAT Leapfrog::step(Simtypes::FLOAT simTime) noexcept {
    Simtypes::FLOAT dt2;
    Simtypes::v3    tmp_position;

    for(Simtypes::SIZE i = 0; i < *bodycount; i++) {
        dt2 = dt[i] * dt[i];

		tmp_position = positions[i];
        // tmp_position.pos[0] = positions[i].pos[0];
        // tmp_position.pos[1] = positions[i].pos[1];
        // tmp_position.pos[2] = positions[i].pos[2];

        positions[i].pos[0] = 2 * positions[i].pos[0] -
                              old_positions[i].pos[0] +
                              acceleration[i].pos[0] * dt2;
        positions[i].pos[1] = 2 * positions[i].pos[1] -
                              old_positions[i].pos[1] +
                              acceleration[i].pos[1] * dt2;
        positions[i].pos[2] = 2 * positions[i].pos[2] -
                              old_positions[i].pos[2] +
                              acceleration[i].pos[2] * dt2;

		old_positions[i] = tmp_position;
        // old_positions[i].pos[0] = tmp_position.pos[0];
        // old_positions[i].pos[1] = tmp_position.pos[1];
        // old_positions[i].pos[2] = tmp_position.pos[2];

        // reset acceleration to zero for the next step
        acceleration[i].pos[0] = 0;
        acceleration[i].pos[1] = 0;
        acceleration[i].pos[2] = 0;
    }

    return simTime;
}
