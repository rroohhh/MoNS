#ifndef _DEBUGONLY_H
#define _DEBUGONLY_H

// some convenience macros

#ifdef NDEBUG

#define DEBUGONLY(args)

#else

#define DEBUGONLY(args) args

#endif


#endif
