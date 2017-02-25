#ifndef __COMMANDLINEOPTIONS_H_
#define __COMMANDLINEOPTIONS_H_

#include <argp.h>

#include "config.h"
#include "types.h"

namespace mons {
    class cli_options {
    public:
        cli_options(int argc, char ** argv) noexcept {
            argp_parse(&argp, argc, argv, 0, nullptr, &args);
        }

        string config_file() noexcept { return args.input_file; }

        bool gui() noexcept { return args.gui; }
        bool ssh() noexcept { return args.ssh; }

    private:
        // documentation of the program
        string doc = "MoNS -- a modular nbody solver";

        // documentation of non option arguments
        char args_doc[255] = "CONFIGFILE";

        // supported options
        struct argp_option options[3] = {
            {"gui", 'g', nullptr, 0, "Produce graphical output", {}},
            {"nossh", 's', nullptr, 0, "disable ssh", {}},
            {}};

        // struct for holding the argument info
        struct arguments {
            bool   gui        = false;
            bool   ssh        = true;
            string input_file = "config.toml";
        };

        // parsing of a single argument
        static error_t parse_opt(int key, char * arg,
                                 struct argp_state * state) {

            // get the struct for saving the arguments
            arguments * arguments = (struct arguments *)state->input;

            switch(key) {
            case 'g': /* use gui */
                arguments->gui = true;
                break;

            case 's': /* disable ssh */
                arguments->ssh = false;
                break;

            case ARGP_KEY_ARG: /* non option argument */
                               // only one argument supportet atm (configfile)
                if(state->arg_num >= 1) { argp_usage(state); }

                arguments->input_file = string(arg);
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
        struct argp argp = {options, parse_opt, args_doc, doc.c_str(),
                            {},      {},        {}};

        arguments args;
    };
}

#endif
