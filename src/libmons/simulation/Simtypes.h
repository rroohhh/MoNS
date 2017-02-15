#ifndef _SIMTYPES_H
#define _SIMTYPES_H

// basic type for the simulation

namespace Simtypes {
    using SIZE = unsigned long;
    using INT = long;
    using FLOAT = double;
    struct v3 {
        FLOAT pos[3];
    };
}

#endif
