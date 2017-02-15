/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  time.h -- system time helper
*/

// use two underscore to not mess with the system time header
#ifndef _SYS_TIME_H
#define _SYS_TIME_H

#include "../util/string_util.h"
#include <ctime>
#include <string>
#include <sys/types.h>

namespace systime {
    inline std::time_t now() noexcept { return std::time(nullptr); }
    inline std::string dateString() noexcept {
        auto        n = now();
        std::string tmp(std::ctime(&n));
        return trim(tmp);
    }
}

#endif
