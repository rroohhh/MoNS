/*
  Copyright (C) 2017 by Robin Heinemann. All Rights Reserved.
  print_tuple.h -- prints a tuple
*/

#ifndef __PRINT_TUPLE_H_
#define __PRINT_TUPLE_H_

#include "../types.h"

namespace mons {
    namespace detail {
        template <typename... Args, usize... S>
        void print_tuple(ostream & os, tuple<Args...> t,
                              index_sequence<S...>) {
            using swallow = i8[];
            (void)swallow{
                0, (void(os << (S == 0 ? "" : ", ") << get<S>(t)), 1)...};
        }
    }

    template <typename... Args>
    ostream & operator<<(ostream & os, tuple<Args...> t) {
        os << "(";
        detail::print_tuple(os, t, index_sequence_for<Args...>());
        return os << ")";
    }
}

#endif
