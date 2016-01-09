#ifndef _DEBUGOUTPUT_H
#define _DEBUGOUTPUT_H

#include <iostream>

#ifdef NDEBUG
#define DEBUGOUTPUT(args)
#else
#define DEBUGOUTPUT(args) std::cout << args
#endif

#endif
