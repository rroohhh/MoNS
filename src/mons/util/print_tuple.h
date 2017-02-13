/*
  Copyright (C) 2017 by Robin Heinemann. All Rights Reserved.
  print_tuple.h -- prints a tuple
*/

#ifndef _PRINT_TUPLE_H
#define _PRINT_TUPLE_H

#include <functional>
#include <ostream>

namespace detail {
    template <typename... Args, size_t... S>
    std::ostream & print_tuple(std::ostream & os, std::tuple<Args...> t,
                               std::index_sequence<S...>) {
        using swallow = int[];
        (void)swallow{
            0, (void(os << (S == 0 ? "" : ", ") << std::get<S>(t)), 1)...};
    }
}

template <typename... Args>
std::ostream & operator<<(std::ostream & os, std::tuple<Args...> t) {
    os << "(";
    detail::print_tuple(os, t, std::index_sequence_for<Args...>());
    return os << ")";
}

#endif
