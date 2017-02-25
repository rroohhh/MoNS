/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  SimulationSettings.h -- Stores all simulation settings
*/

#ifndef __SIMULATIONSETTINGS_H_
#define __SIMULATIONSETTINGS_H_

#include "types.h"

namespace mons {
    struct simulation_settings {
        size bodycount;
        f    endTime;
        f    dt;
        f *  masses;
        v3 * positions;
        v3 * velocities;
    };
}

#endif
