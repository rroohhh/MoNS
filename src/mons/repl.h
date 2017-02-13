/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  repl.h -- Interactive simulation control
*/

#ifndef _REPL_H
#define _REPL_H

#include "CommandHandler.h"
#include "io/log.h"
#include "platform/Platform.h"
#include "readline.h"
#include "string_helper.h"
#include <cstdio>
#include <thread>

template <typename T, typename G>
class repl {
public:
    repl(T io, CommandHandler<G> handler) noexcept {
        auto t = new std::thread([=]() {
            int         length;
            readline<T> rl(1024, io);
            auto        prompt = "λ ";
            rl.add_completer(handler.completer());

            io::log::debug("repl: new user connected");

            auto line = rl.getline(prompt);

            while(line) {
                auto l = line->unpack();
                if(!whitespace_only(l)) {
                    auto result = handler.run(*line);

                    if(!whitespace_only(result.output)) {
                        io_stream<T>(io).write(result.output + "\n");
                    }

                    rl.history_add(*line);
                }

                line = rl.getline(prompt);
            }

            rl.finish();
        });
    }

private:
};

#endif
