/*
  Copyright (C) 2017 by Robin Heinemann. All Rights Reserved.
  random.h -- random
*/

#ifndef __RANDOM_H_
#define __RANDOM_H_

#include <random>

namespace mons {
	using std::mt19937;
	using std::uniform_int_distribution;
	using std::uniform_real_distribution;
	using std::random_device;
}

#endif
