/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  string_helper.h -- various string help routines
*/

#ifndef _STRING_HELPER_H
#define _STRING_HELPER_H

#include <algorithm>
#include <cctype>

template <typename T>
bool whitespace_only(const T & string) {
	auto f = [](unsigned char const c) { return std::isspace(c); };
	return std::all_of(string.begin(), string.end(), f);
}

#endif
