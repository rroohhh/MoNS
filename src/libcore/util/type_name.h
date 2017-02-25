/*
  Copyright (C) 2017 by Robin Heinemann. All Rights Reserved.
  type_name.h -- get the name of a type as string
*/

#ifndef __TYPE_NAME_H_
#define __TYPE_NAME_H_

#include "static_string.h"

namespace mons {
    template <typename T>
    constexpr static_string type_name() {
        static_string name = __PRETTY_FUNCTION__;
        return static_string(name.data() + 45, name.size() - 46);

        /*
         * #ifdef __clang__
         *     return static_string(name.data() + 31, name.size() - 32);
         * #elif defined(__GNUC__)
         * #if __cplusplus < 201402
         *     return static_string(name.data() + 36, name.size() - 37);
         * #else
         *     return static_string(name.data() + 55, name.size() - 56);
         * #endif
         * #endif
         */
    }
}

#endif
