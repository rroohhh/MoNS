/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  repl.cpp -- Interactive simulation control
*/

#include "repl.h"

repl::repl(FILE * in, FILE * out, CommandHandler handler,
           const char * prompt) noexcept {
    auto t = new std::thread([&]() {
        // DEBUGGING ONLY!!!!!
        if(!Platform::fork()) {
            init_readline(in, out);
            // char * line;
            char buf[1];

            printf("yay?\n");

            while(!(read(fileno(in), buf, 1))) {
				write(1, buf, 1);
			}
            // while((line = readline(prompt))) {
            //     printf("yay!\n");
            //     add_history(line);
            //     free(line);
            // }

            printf("nay :(\n");

            fclose(in);
            fclose(out);

            exit(EXIT_SUCCESS);
        }
    });
}

inline void repl::init_readline(FILE * infile, FILE * outfile) noexcept {
    rl_instream  = infile;
    rl_outstream = outfile;

    rl_editing_mode = 0;
}
