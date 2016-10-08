/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  repl.h -- Interactive simulation control
*/

#ifndef _REPL_H
#define _REPL_H

#include "CommandHandler.h"
#include "platform/Platform.h"
#include <cstdio>
#include "io/log.h"
//#include <readline/history.h>
//#include <readline/readline.h>
#include "histedit.h"
#include <thread>

class repl {
public:
    repl(FILE * in, FILE * out, CommandHandler handler) noexcept;

private:
    void init_editline(FILE * infile, FILE * outfile) noexcept;
    EditLine * el = NULL;
    History *  hist;
    HistEvent  ev;
};

#endif
