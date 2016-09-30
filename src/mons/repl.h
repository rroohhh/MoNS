/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  repl.h -- Interactive simulation control
*/

#ifndef _REPL_H
#define _REPL_H

#include "CommandHandler.h"
#include "platform/Platform.h"
#include <cstdio>
#include <readline/history.h>
#include <readline/readline.h>
#include <thread>

class repl {
public:
    repl(FILE * in, FILE * out, CommandHandler handler,
         const char * prompt = "λ ") noexcept;

private:
    static inline void init_readline(FILE * infile, FILE * outfile) noexcept;
};

#endif
