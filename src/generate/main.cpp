/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  main.cpp -- Generate different types of initial conditions
*/
#include "io/log.h"
#include "simulation/SimulationConfig.h"

int main() {
    Simtypes::FLOAT masses[]     = {1, 1, 200000};
    Simtypes::v3    positions[]  = {{{0, 0, 0}}, {{50, 0, 0}}, {{100, 0, 0}}};
    Simtypes::v3    velocities[] = {
        {{0, 0.0004, 0}}, {{0, 0.0006, 0}}, {{0, 0, 0}}};
    SimulationConfig(3, 100000000, 0.1, masses, positions, velocities)
        .write_to_file("config", "data");
    // io::log::info("Hello, world!");
    return 0;
}
