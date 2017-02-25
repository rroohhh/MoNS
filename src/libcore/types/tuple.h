/*
  Copyright (C) 2017 by Robin Heinemann. All Rights Reserved.
  tuple.h -- tuple
*/

#ifndef __TUPLE_H_
#define __TUPLE_H_

#include <tuple>

namespace mons {
	template <typename... Ts>
	using tuple = std::tuple<Ts...>;
	using std::tuple_element;
	using std::make_tuple;

	using std::get;
	using std::index_sequence;
	using std::index_sequence_for;
}

#endif
