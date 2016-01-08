#ifndef _DEBUGOUTPUT_H
#define _DEBUGOUTPUT_H


#ifdef NDEBUG
#define DEBUGOUTPUT(args)
#else
#define DEBUGOUTPUT(args) (std::cout << args)
#endif

#endif
