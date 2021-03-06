/*
  Copyright (C) 2017 by . All Rights Reserved.
  make_function.cpp -- make_function helper
*/

#ifndef __MAKE_FUNCTION_H_
#define __MAKE_FUNCTION_H_

#include "../types.h"

namespace mons {
    // For generic types that are functors, delegate to its 'operator()'
    template <typename T>
    struct function_traits : public function_traits<decltype(&T::operator())> {
    };

    // for pointers to member function
    template <typename ClassType, typename ReturnType, typename... Args>
    struct function_traits<ReturnType (ClassType::*)(Args...) const> {
        enum { arity = sizeof...(Args) };
        using f_type = function<ReturnType(Args...)>;
    };

    // for pointers to member function
    template <typename ClassType, typename ReturnType, typename... Args>
    struct function_traits<ReturnType (ClassType::*)(Args...)> {
        enum { arity = sizeof...(Args) };
        using f_type = function<ReturnType(Args...)>;
    };

    // for function pointers
    template <typename ReturnType, typename... Args>
    struct function_traits<ReturnType (*)(Args...)> {
        enum { arity = sizeof...(Args) };
        using f_type = function<ReturnType(Args...)>;
    };

    template <typename L>
    static typename function_traits<L>::f_type make_function(L l) {
        return (typename function_traits<L>::f_type)(l);
    }

    // handles bind & multiple function call operator()'s
    template <typename ReturnType, typename... Args, class T>
    auto make_function(T && t)
        -> function<decltype(ReturnType(t(declval<Args>()...)))(Args...)> {
        return {forward<T>(t)};
    }

    // handles explicit overloads
    template <typename ReturnType, typename... Args>
    auto make_function(ReturnType (*p)(Args...))
        -> function<ReturnType(Args...)> {
        return {p};
    }

    // handles explicit overloads
    template <typename ReturnType, typename... Args, typename ClassType>
    auto make_function(ReturnType (ClassType::*p)(Args...))
        -> function<ReturnType(Args...)> {
        return {p};
    }
}

#endif
