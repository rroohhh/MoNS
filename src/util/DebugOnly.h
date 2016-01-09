#ifndef _DEBUGONLY_H
#define _DEBUGONLY_H

#ifdef NDEBUG

#define DEBUGONLY(args)

#else

#define DEBUGONLY(args) args

#endif


#endif
