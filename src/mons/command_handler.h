/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  CommandHandler.h -- Handles user commands
*/

#ifndef __COMMANDHANDLER_H_
#define __COMMANDHANDLER_H_

#include "bench.h"
#include "command.h"
#include "readline.h"
#include "utf8.h"
#include "util.h"

#include "fmt/format.h"

namespace mons {
    template <typename Sim>
    class command_handler {
    public:
        command_handler(Sim * sim) : sim(sim) {
            cmds["status"] = make_command([=]() {
                auto diff =
                    high_resolution_clock::now() - sim->starting_time();

                diff *= sim->sim_config()->total_steps() / sim->steps();
                auto date = high_resolution_clock::now() + diff;
                auto time = high_resolution_clock::to_time_t(date);

                return trimmed(fmt::format(
                    "current step: {}, total steps: {}, eta: {}", sim->steps(),
                    sim->sim_config()->total_steps(), ctime(&time)));
            });

            cmds["stats"] = make_command(
                []() { return rtrimmed(timed_block::format_timings()); });

            cmds["quit"] = make_command([]() { exit(EXIT_SUCCESS); return ""; });
        }

        function<vector<utf8string>(const utf8string s)> completer() const {
            vector<utf8string> possible_canidates;

            for(const auto & kv : cmds) {
                possible_canidates.push_back(kv.first.c_str());
            }

            return complete_from_canidates(possible_canidates);
        }

        command_result run(const utf8string & s) const {
            auto cmd = split(s.unpack());
            if(cmds.find(cmd[0]) == cmds.end()) {
                return make_pair(string("Unknown command ") + cmd[0] + "!", -1);
            }
            vector<string> args;
            if(cmd.size() >= 2) {
                args.insert(args.end(), cmd.begin() + 1, cmd.end());
            }

            try {
                return cmds.at(cmd[0])(args);
            } catch(string e) {
                auto strs = split(e, '\n');
                return make_pair(
                    strs[0] + "\r\n" + cmd[0] + "(" + strs[1] + ")", -1);
            }
        }

    private:
        Sim * sim;
        map<string, command> cmds;
    };
}

#endif
