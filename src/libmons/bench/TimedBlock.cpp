/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  TimedBlock.cpp -- a timed block, that times a scope execution
*/

#include "TimedBlock.h"

std::unordered_map<std::string, TimedBlock::timing_record> TimedBlock::timings;
