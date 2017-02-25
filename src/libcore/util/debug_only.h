#ifndef __DEBUGONLY_H_
#define __DEBUGONLY_H_

// some convenience macros

#ifdef NDEBUG

#define DEBUGONLY(args)

#else

#define DEBUGONLY(args) args

#endif

#endif
