/*
  Copyright (C) 2017 by Robin Heinemann. All Rights Reserved.
  to_tuple.h -- parse a vector to a tuple
*/

#ifndef __TO_TUPLE_H_
#define __TO_TUPLE_H_

#include "../types.h"
#include "type_name.h"

namespace mons {
    namespace detail {
        template <template <typename> class F, typename... Args, typename T,
                  usize... S>
        tuple<Args...> to_tuple(const vector<T> & v,
                                index_sequence<S...> /*unused*/) {
            return make_tuple((F<Args>()(v[S]))...);
        }

        template <typename... Args, usize... S>
        string string_arg_pack(index_sequence<S...> /*unused*/) {
            using swallow = i8[];
            stringstream ss;
            (void)swallow{0, (void(ss << (S == 0 ? "" : ", ")
                                      << type_name<typename tuple_element<
                                             S, tuple<Args...>>::type>()),
                              1)...};

            return ss.str();
        }
    }

    template <typename... Args>
    string string_arg_pack() {
        return detail::string_arg_pack<Args...>(index_sequence_for<Args...>());
    }

    template <template <typename> class F, typename... Args, typename T>
    tuple<Args...> to_tuple(const vector<T> & v) {
        if(v.size() != sizeof...(Args)) {
            throw string("invalid number of arguments, needed: ") +
                to_string(sizeof...(Args)) + "\n" + string_arg_pack<Args...>();
        }

        return detail::to_tuple<F, Args...>(v, index_sequence_for<Args...>());
    }
}

#endif
