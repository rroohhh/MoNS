#ifndef _DEBUGOUTPUT_H
#define _DEBUGOUTPUT_H

#include <iostream>

// some convenience macros

#ifdef NDEBUG
#define DEBUGOUTPUT(args)
#else
#define DEBUGOUTPUT(args) std::cout << args
#endif

#endif
