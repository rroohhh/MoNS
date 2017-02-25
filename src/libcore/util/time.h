/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  time.h -- system time helper
*/

// use two underscore to not mess with the system time header
#ifndef __SYS_TIME_H_
#define __SYS_TIME_H_

#include "../types.h"
#include "../util/string_util.h"

#include <ctime>

namespace mons {
    namespace systime {
        inline time_t now() noexcept { return time(nullptr); }
        inline string dateString() noexcept {
            auto   n = now();
            string tmp(ctime(&n));
            return trim(tmp);
        }
    }
}

#endif
