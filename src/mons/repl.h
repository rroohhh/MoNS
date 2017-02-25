/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  repl.h -- Interactive simulation control
*/

#ifndef __REPL_H_
#define __REPL_H_

#include "command_handler.h"
#include "io.h"
#include "platform.h"
#include "readline.h"
#include "string_helper.h"
#include "types.h"

namespace mons {
    template <typename T, typename G>
    class repl {
    public:
        repl(T io, command_handler<G> handler, bool close = true) noexcept {
            auto t = new thread([=]() {
                i32         length;
                readline<T> rl(1024, io);
                auto        prompt = "λ ";
                rl.add_completer(handler.completer());

                log::debug("repl: new user connected");

                auto line = rl.getline(prompt);

                while(line || !close) {
                    if(line) {
                        auto l = line->unpack();
                        if(!whitespace_only(l)) {
                            log::debug("repl: got line {}", l);
                            auto result = handler.run(*line);

                            log::debug("repl: result {}", result.output);

                            if(!whitespace_only(result.output)) {
                                io_stream<T>(io).write(result.output + "\n");
                            }

                            rl.history_add(*line);
                        }
                    }

                    log::debug("repl: getting new line");
                    line = rl.getline(prompt);
                }

                rl.finish();
                log::debug("repl: closing");
            });
        }

    private:
    };
}

#endif
