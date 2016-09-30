/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  TimedBlock.h -- a timed block, that times a scope execution
*/

#ifndef _TIMEDBLOCK_H
#define _TIMEDBLOCK_H

#include <chrono>
#include <string>
#include <unordered_map>

#include "../io/log.h"
#include "../simulation/Simtypes.h"
#include "../platform/Platform.h"

using namespace io;

#define PASTE_(a, b) a##b
#define PASTE(a, b) PASTE_(a, b)
#define TIMED_BLOCK(name) TimedBlock PASTE(TimedBlock, name)(#name)
#define END_BLOCK(name) PASTE(TimedBlock, name).end();

class TimedBlock {
    typedef std::chrono::high_resolution_clock clock;
    typedef std::chrono::time_point<clock>     time_point;
    typedef std::chrono::nanoseconds           duration;

    struct timing_record {
        duration       dur;
        Simtypes::SIZE count;
    };

public:
    TimedBlock(std::string name) noexcept : start(clock::now()), name(name) {}

    void end() noexcept {
        if(!ended) {
            auto   duration = clock::now() - start;
            auto & record   = TimedBlock::timings[name];
            record.dur += duration;
            record.count++;
        }
    }

    ~TimedBlock() noexcept { end(); }

    static void output_timings() noexcept {
        log::info("{:>20}{:>20}{:>20}", "name", "total", "avg");
        for(const auto & record : timings) {
            log::info("{:>20}{:>18}ns{:>18}ns", record.first,
                      record.second.dur.count(),
                      record.second.dur.count() / record.second.count);
        }
    }

    static std::unordered_map<std::string, timing_record> timings;

private:
    time_point  start;
    std::string name;
    bool        ended = false;
};

#endif
