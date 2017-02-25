/*
  Copyright (C) 2017 by Robin Heinemann. All Rights Reserved.
  process.h -- process handling
*/

#ifndef __PROCESS_H_
#define __PROCESS_H_

#include <csignal>
#include <unistd.h>
#include "../types.h"

namespace mons {
    namespace platform {
        inline static i32 fork() {
            auto ret = ::fork();
            if(ret == 0) {
                struct sigaction act {};
                act.sa_handler = SIG_DFL;
                sigemptyset(&act.sa_mask);
                act.sa_flags = 0;
                sigaction(SIGINT, &act, nullptr);
            }

            return ret;
        }
    }
}

#endif
