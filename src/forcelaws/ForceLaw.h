#ifndef _FORCELAW_H
#define _FORCELAW_H

#include "../simulation/Simtypes.h"

// interface for a forcelaw

// the forcelaw applies a law of force to a set of particles
// and also does collision detection

class ForceLaw {
public:
	virtual void apply(Simtypes::FLOAT simTime) = 0;
	virtual ~ForceLaw() {};
};

#endif
