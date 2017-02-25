/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  TimedBlock.cpp -- a timed block, that times a scope execution
*/

#include "timed_block.h"

namespace mons {
    map<string, timed_block::timing_record> timed_block::timings;
}
