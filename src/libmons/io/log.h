/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  log.h -- logging facilities
*/

#ifndef _LOG_H
#define _LOG_H

#include "fmt/format.h"

#include "../util/time.h"

#include <cstdio>
#include <iostream>
#include <unordered_map>

namespace io {
    namespace log {
        static std::unordered_map<std::string, FILE *>
            log_outputs({{"stdout", stdout}});

        namespace detail {
            template <typename... Args>
            static inline std::string log_format(std::string identifier,
                                                 Args... args) {
                return fmt::format("[{}][{}] {}\n", identifier,
                                   systime::dateString(), fmt::format(args...));
            }

            template <typename... Args>
            static inline void log_to_all(std::string identifier,
                                          Args... args) {
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
#ifndef NDEBUG
            detail::log_to_all("D", args...);
#endif
        }
    }
}

#endif
