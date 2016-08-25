#ifndef _COMMANDLINEOPTIONS_H
#define _COMMANDLINEOPTIONS_H

#include <argp.h>
#include <string>

#include "../config.h"

class CommandLineOptions {
public:
    CommandLineOptions(int argc, char ** argv);

    std::string configFile();
    bool        gui();

private:
    // documentation of the program
    std::string doc = "MoNS -- a modular nbody solver";

    // documentation of non option arguments
    char args_doc[255] = "CONFIGFILE";

    // supported options
    struct argp_option options[2] = {
        {"gui", 'g', 0, 0, "Produce graphical output"}, {0}};

    // struct for holding the argument info
    struct arguments {
        bool        gui;
        std::string input_file;
    };

    // parsing of a single argument
    static error_t parse_opt(int key, char * arg, struct argp_state * state) {

        // get the struct for saving the arguments
        arguments * arguments = (struct arguments *)state->input;

        switch(key) {
        case 'g': /* use gui */
            arguments->gui = true;
            break;

        case ARGP_KEY_ARG: /* non option argument */
                           // only one argument supportet atm (configfile)
            if(state->arg_num >= 1) { argp_usage(state); }

            arguments->input_file = std::string(arg);
            break;

        case ARGP_KEY_END: /* end of arguments */
            // check for too few non option arguments
            if(state->arg_num < 1) { argp_usage(state); }
            break;

        default:
            return ARGP_ERR_UNKNOWN;
        }
        return 0;
    }

    // argp parser definiton
    struct argp argp = {options, parse_opt, args_doc, doc.c_str()};

    arguments args;
};

#endif
