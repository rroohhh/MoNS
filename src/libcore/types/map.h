/*
  Copyright (C) 2017 by Robin Heinemann. All Rights Reserved.
  map.h -- map type
*/

#ifndef __MAP_H_
#define __MAP_H_

#include <unordered_map>
#include <map>

namespace mons {
	template<typename T, typename G>
	using map = std::unordered_map<T, G>;

	template<typename T, typename G>
	using ordered_map = std::map<T, G>;
}

#endif
