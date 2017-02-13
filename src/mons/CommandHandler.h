/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  CommandHandler.h -- Handles user commands
*/

#ifndef _COMMANDHANDLER_H
#define _COMMANDHANDLER_H

#include "Command.h"
#include "readline.h"
#include "utf8.h"
#include "util/split.h"

#include <unordered_map>

template <typename Sim>
class CommandHandler {
public:
    CommandHandler(Sim * sim) : sim(sim) {
        cmds["test1"] = make_command(
            [&](int i, double d) { return std::to_string((double)i / d); });
        cmds["test2"] =
            make_command([&](std::string s) { return "test2 " + s; });
        cmds["test3"] =
            make_command([&]() { return std::make_pair("test3", -1); });
        cmds["test4"] = make_command([&](int i, double d) { return; });
    }

    std::function<std::vector<utf8string>(const utf8string s)>
    completer() const {
        std::vector<utf8string> possible_canidates;

        for(const auto & kv : cmds) {
            possible_canidates.push_back(kv.first.c_str());
        }

        return complete_from_canidates(possible_canidates);
    }

    CommandResult run(utf8string s) const {
        auto cmd = split(s.unpack(), ' ');
        if(cmds.find(cmd[0]) == cmds.end()) {
            return std::make_pair(
                std::string("Unknown command ") + cmd[0] + "!", -1);
        } else {
            std::vector<std::string> args;
            if(cmd.size() >= 2) {
                args.insert(args.end(), cmd.begin() + 1, cmd.end());
            }

            try {
                return cmds.at(cmd[0])(args);
            } catch(std::string e) {
				auto strs = split(e, '\n');
				return std::make_pair(strs[0] + "\r\n" + cmd[0] + "(" + strs[1] + ")", -1); }
        }
    }

private:
    Sim * sim;
    std::unordered_map<std::string, Command> cmds;
};

#endif
