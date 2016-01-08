#ifndef _FORCELAW_H
#define _FORCELAW_H

#include "../simulation/Simtypes.h"

class ForceLaw {
public:
	virtual void apply(Simtypes::FLOAT simTime) = 0;
	virtual ~ForceLaw() {};
};

#endif
