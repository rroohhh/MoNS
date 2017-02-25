/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  main.cpp -- Generate different types of initial conditions
*/

#include "io.h"
#include "platform.h"
#include "simulation/simulation_config.h"
#include "constants.h"

// ToDo(robin): add config options

#define count 100000
#define min 1
#define max 200
#define dt 0.1
#define endtime 100000000
#define cmass 200000
#define mass 1

using namespace mons;

int main() {
    mt19937 rng;
    rng.seed(random_device()());
    uniform_real_distribution<double> angle(0, 360);
    uniform_real_distribution<double> radius(min, max);

    auto masses     = platform::allocate<f>(count);
    auto positions  = platform::allocate<v3>(count);
    auto velocities = platform::allocate<v3>(count);

    int i = 0;

    // central body
    masses[i]            = cmass;
    positions[i].pos[0]  = 0;
    positions[i].pos[1]  = 0;
    positions[i].pos[2]  = 0;
    velocities[i].pos[0] = 0;
    velocities[i].pos[1] = 0;
    velocities[i].pos[2] = 0;

    // the other / small bodies
    for(i++; i < count; i++) {
        auto r = radius(rng);
        auto a = angle(rng);

        masses[i] = mass;

        positions[i].pos[0] = r * cos(a);
        positions[i].pos[1] = r * -sin(a);
        positions[i].pos[2] = 0;

        velocities[i].pos[0] =
            sin(a) * sqrt((constants::physics::gamma * cmass) / r);
        velocities[i].pos[1] =
            cos(a) * sqrt((constants::physics::gamma * cmass) / r);
        velocities[i].pos[2] = 0;
    }

    simulation_config(count, endtime, dt, masses, positions, velocities)
        .write_to_file("config", "data");
    return 0;
}
