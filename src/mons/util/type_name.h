/*
  Copyright (C) 2017 by Robin Heinemann. All Rights Reserved.
  type_name.h -- get the name of a type as string
*/

#ifndef _TYPE_NAME_H
#define _TYPE_NAME_H

#include "static_string.h"

template <typename T>
constexpr StaticString type_name() {
    StaticString name = __PRETTY_FUNCTION__;
	return StaticString(name.data() + 45, name.size() - 46);
/*
 * #ifdef __clang__
 *     return StaticString(name.data() + 31, name.size() - 32);
 * #elif defined(__GNUC__)
 * #if __cplusplus < 201402
 *     return StaticString(name.data() + 36, name.size() - 37);
 * #else
 *     return StaticString(name.data() + 55, name.size() - 56);
 * #endif
 * #endif
 */
}

#endif
