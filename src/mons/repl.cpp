/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  repl.cpp -- Interactive simulation control
*/

#include "repl.h"

repl::repl(FILE * in, FILE * out, CommandHandler handler) noexcept {
    auto t = new std::thread([=]() {
        init_editline(in, out);
        const char * line;
        int          length;

        io::log::info("init done");
        while((line = el_gets(el, &length))) {
            io::log::info("got line: {}", line);
            // io::log::info("el->el_flags | NO_TTY {}", el->el_flags&0x02);
            if(length > 1) { history(hist, &ev, H_ENTER, line); }
        }

        history_end(hist);
        el_end(el);

        fclose(in);
        fclose(out);
    });
}

char * prompt(EditLine * el) { return "λ "; }

void repl::init_editline(FILE * infile, FILE * outfile) noexcept {
    setlocale(LC_CTYPE, "");

    hist = history_init();
    history(hist, &ev, H_SETSIZE, 100);

    el = el_init("./MoNS", infile, outfile, outfile);
    el_set(el, EL_EDITOR, "vi");
    // el_set(el, EL_PROMPT_ESC,
    //        +[](EditLine *el) -> const char * {
    //          const char *prompt = "λ ";
    //          return prompt;
    //        },
    //        '\1');
    el_set(el, EL_PROMPT_ESC, &prompt, '\1');
    el_set(el, EL_HIST, history, hist);
    el_source(el, NULL);
}
