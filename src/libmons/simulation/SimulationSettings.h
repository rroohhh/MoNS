/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  SimulationSettings.h -- Stores all simulation settings
*/

#ifndef _SIMULATIONSETTINGS_H
#define _SIMULATIONSETTINGS_H

#include "Simtypes.h"

struct SimulationSettings {
    Simtypes::SIZE    bodycount;
    Simtypes::FLOAT   endTime;
    Simtypes::FLOAT   dt;
    Simtypes::FLOAT * masses;
    Simtypes::v3 *    positions;
    Simtypes::v3 *    velocities;
};

#endif
