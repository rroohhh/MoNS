/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  repl.cpp -- Interactive simulation control
*/

#include "repl.h"

repl::repl(FILE * in, FILE * out, CommandHandler handler,
           const char * prompt) noexcept {
    auto t = new std::thread([=]() {
        // DEBUGGING ONLY!!!!!
        // if(!Platform::fork()) {
            init_readline(in, out);
            char * line;

            while((line = readline(prompt))) {
                add_history(line);
                free(line);
            }

            fclose(in);
            fclose(out);

            exit(EXIT_SUCCESS);
        // } else { // Need to close these because of shit... and linux...
        //     fclose(in);
        //     fclose(out);
        // }
    });
}

inline void repl::init_readline(FILE * infile, FILE * outfile) noexcept {
    rl_instream  = infile;
    rl_outstream = outfile;

    rl_editing_mode = 0;
}
