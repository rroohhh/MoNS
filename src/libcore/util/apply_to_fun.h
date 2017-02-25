/*
  Copyright (C) 2017 by Robin Heinemann. All Rights Reserved.
  apply_to_fun.h -- apply a tuple to a function
*/

#ifndef __APPLY_TO_FUN_H_
#define __APPLY_TO_FUN_H_

#include "../types.h"

namespace mons {
    namespace detail {
        template <typename F, typename... Args, usize... S>
        auto apply_to_fun(F fun, tuple<Args...> t,
                          index_sequence<S...> /*unused*/) {
            // TODO(robin): file a gcc bug about t not being used, although it
            // is
            // used
            (void)t;
            return fun(get<S>(t)...);
        }
    }

    template <typename F, typename... Args>
    auto apply_to_fun(F fun, tuple<Args...> t) {
        return detail::apply_to_fun(fun, t, index_sequence_for<Args...>());
    }
}

#endif
