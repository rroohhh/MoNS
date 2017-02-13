/*
  Copyright (C) 2017 by Robin Heinemann. All Rights Reserved.
  apply_to_fun.h -- apply a tuple to a function
*/

#ifndef _APPLY_TO_FUN_H
#define _APPLY_TO_FUN_H

#include <functional>

namespace detail {
    template <typename F, typename... Args, size_t... S>
    auto apply_to_fun(F fun, std::tuple<Args...> t, std::index_sequence<S...>) {
		// TODO(robin): file a gcc bug about t not being used, although it is used
		(void)t;
        return fun(std::get<S>(t)...);
    }
}

template <typename F, typename... Args>
auto apply_to_fun(F fun, std::tuple<Args...> t) {
    return detail::apply_to_fun(fun, t, std::index_sequence_for<Args...>());
}

#endif
