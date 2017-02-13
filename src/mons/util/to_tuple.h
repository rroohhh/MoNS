/*
  Copyright (C) 2017 by Robin Heinemann. All Rights Reserved.
  to_tuple.h -- parse a vector to a tuple
*/

#ifndef _TO_TUPLE_H
#define _TO_TUPLE_H

#include <functional>
#include <sstream>
#include <vector>

#include "type_name.h"

namespace detail {
    template <template <typename> class F, typename... Args, typename T,
              size_t... S>
    std::tuple<Args...> to_tuple(const std::vector<T> & v,
                                 std::index_sequence<S...>) {
        return std::make_tuple((F<Args>()(v[S]))...);
    }

    template <typename... Args, size_t... S>
    std::string string_arg_pack(std::index_sequence<S...>) {
        using swallow = int[];
        std::stringstream ss;
        (void)swallow{0, (void(ss << (S == 0 ? "" : ", ")
                                  << type_name<typename std::tuple_element<
                                         S, std::tuple<Args...>>::type>()),
                          1)...};

        return ss.str();
    }
}

template <typename... Args>
std::string string_arg_pack() {
    return detail::string_arg_pack<Args...>(std::index_sequence_for<Args...>());
}

template <template <typename> class F, typename... Args, typename T>
std::tuple<Args...> to_tuple(const std::vector<T> & v) {
    if(v.size() != sizeof...(Args)) {
        throw std::string("invalid number of arguments, needed: ") +
            std::to_string(sizeof...(Args)) + "\n" + string_arg_pack<Args...>();
    }

    return detail::to_tuple<F, Args...>(v, std::index_sequence_for<Args...>());
}

#endif
