/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  log.h -- logging facilities
*/

#ifndef __LOG_H_
#define __LOG_H_

#include "../types.h"
#include "../util/debug_only.h"
#include "../util/time.h"
#include "fmt/format.h"
#include <cstdio>

namespace mons {
    namespace log {
        static map<string, FILE *> log_outputs({{"stdout", stdout}});

        namespace detail {
            template <typename... Args>
            static inline string log_format(string identifier, Args... args) {
                return fmt::format("[{}][{}] {}\n", identifier,
                                   systime::dateString(), fmt::format(args...));
            }

            template <typename... Args>
            static inline void log_to_all(string identifier, Args... args) {
                auto log_line = detail::log_format(identifier, args...);
                for(auto file : log_outputs) {
                    fmt::print(file.second, "{}", log_line);
                }
            }
        }

        template <typename... Args>
        static inline void info(Args... args) {
            detail::log_to_all("I", args...);
        }

        template <typename... Args>
        static inline void err(Args... args) {
            detail::log_to_all("E", args...);
        }

        template <typename... Args>
        static inline void warn(Args... args) {
            detail::log_to_all("W", args...);
        }

        template <typename... Args>
        static inline void debug(Args... args) {
            DEBUGONLY(detail::log_to_all("D", args...));
        }
    }
}

#endif
