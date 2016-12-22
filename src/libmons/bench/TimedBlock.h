/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  TimedBlock.h -- a timed block, that times a scope execution
*/

#ifndef _TIMEDBLOCK_H
#define _TIMEDBLOCK_H

#include <string>
#include <unordered_map>

#include "../io/log.h"
#include "../platform/Platform.h"
#include "../simulation/Simtypes.h"

using namespace io;
#define PASTE_(a, b) a##b
#define PASTE(a, b) PASTE_(a, b)
#define TIMED_BLOCK(name) TimedBlock PASTE(TimedBlock, name)(#name)
#define END_BLOCK(name) PASTE(TimedBlock, name).end();

class TimedBlock {
    struct timing_record {
        uint64_t       dur;
        Simtypes::SIZE count;
    };

public:
    TimedBlock(std::string name) noexcept : start(Platform::rdtsc()), name(name) {}

    void end() noexcept {
        if(!ended) {
            auto   duration = Platform::rdtsc() - start;
            auto & record   = TimedBlock::timings[name];
            record.dur += duration;
            record.count++;
        }
    }

    ~TimedBlock() noexcept { end(); }

    static void output_timings() noexcept {
        log::info("{:>20}{:>20}{:>20}", "name", "total", "avg");
        for(const auto & record : timings) {
            log::info("{:>20}{:>18}cy{:>18}cy", record.first, record.second.dur,
                      record.second.dur / record.second.count);
        }
    }

    static std::unordered_map<std::string, timing_record> timings;

private:
    uint64_t  start;
    std::string name;
    bool        ended = false;
};

#endif
