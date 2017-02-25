/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  string_helper.h -- various string help routines
*/

#ifndef __STRING_HELPER_H_
#define __STRING_HELPER_H_

#include <algorithm>
#include <cctype>
#include <string>

template <typename T>
bool whitespace_only(const T & string) {
    auto f = [](unsigned char const c) { return std::isspace(c); };
    return std::all_of(string.begin(), string.end(), f);
}

#endif
