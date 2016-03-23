#include "CommandLineOptions.h"

// global vars for argp:
const char *argp_program_version = APPLICATION_VERSION_STRING;
const char *argp_program_bug_address = APPLICATION_VENDOR_EMAIL;

CommandLineOptions::CommandLineOptions(int argc, char ** argv) {
	// default values

	args.gui = false;
	args.input_file = "config.toml";

	argp_parse(&argp, argc, argv, 0, 0, &args);
}

std::string CommandLineOptions::configFile() {
	return args.input_file;
}

bool CommandLineOptions::gui() {
	return args.gui;
}
