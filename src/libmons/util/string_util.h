/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  string_util.h -- string utilities
*/

#ifndef _STRING_UTIL_H
#define _STRING_UTIL_H

#include <algorithm>
#include <functional>
#include <sstream>
#include <string>

static inline std::string & ltrim(std::string & s) noexcept {
    s.erase(s.begin(),
            std::find_if(s.begin(), s.end(),
                         std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

static inline std::string & rtrim(std::string & s) noexcept {
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         std::not1(std::ptr_fun<int, int>(std::isspace)))
                .base(),
            s.end());
    return s;
}

static inline std::string & trim(std::string & s) noexcept { return ltrim(rtrim(s)); }
static inline std::string & surround_by(std::string & s, std::string & surrounding) noexcept {
    s.insert(0, surrounding);
    s.append(surrounding);

    return s;
}

template <typename T>
static inline std::string str(T t) noexcept {
    std::ostringstream strs;
    strs << t;
    return strs.str();
}

#endif
