/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  timed_block.h -- a timed block, that times a scope execution
*/

#ifndef __TIMEDBLOCK_H_
#define __TIMEDBLOCK_H_

#include "../io/log.h"
#include "../platform.h"
#include "../types.h"

#define PASTE_(a, b) a##b
#define PASTE(a, b) PASTE_(a, b)
#define TIMED_BLOCK(name) timed_block PASTE(timed_block, name)(#name)
#define END_BLOCK(name) PASTE(timed_block, name).end();

namespace mons {
    class timed_block {
        struct timing_record {
            u64 dur;
            u64 count;
        };

    public:
        timed_block(string name) noexcept
            : start(platform::rdtsc()), name(move(name)) {}

        void end() noexcept {
            if(!ended) {
                auto   duration = platform::rdtsc() - start;
                auto & record   = timed_block::timings[name];
                record.dur += duration;
                record.count++;
		ended = true;
            }
        }

        ~timed_block() noexcept { end(); }

        static string format_timings() noexcept {
            stringstream ss;
            ss << fmt::format("{:>20}{:>20}{:>20}\n", "name", "total", "avg");
            for(const auto & record : timings) {
                ss << fmt::format("{:>20}{:>18}cy{:>18}cy\n", record.first,
                                  record.second.dur,
                                  record.second.dur / record.second.count);
            }

            return ss.str();
        }

        static void output_timings() noexcept {
            log::info("{:>20}{:>20}{:>20}", "name", "total", "avg");
            for(const auto & record : timings) {
                log::info("{:>20}{:>18}cy{:>18}cy", record.first,
                          record.second.dur,
                          record.second.dur / record.second.count);
            }
        }

        static map<string, timing_record> timings;

    private:
        u64    start;
        string name;
        bool   ended = false;
    };
}

#endif
