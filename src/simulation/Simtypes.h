#ifndef _SIMTYPES_H
#define _SIMTYPES_H

// basic type for the simulation

class Simtypes {
public:
	typedef  unsigned long int SIZE;
	typedef  double FLOAT;
	typedef union {
		struct {
			FLOAT x;
			FLOAT y;
			FLOAT z;
		};
		FLOAT pos[3];
	} v3;
};

#endif
