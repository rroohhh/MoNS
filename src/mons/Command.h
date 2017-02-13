/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  Command.h -- wraps one command
*/

#ifndef _COMMAND_H
#define _COMMAND_H

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "util/apply_to_fun.h"
#include "util/make_function.h"
#include "util/to_tuple.h"

using namespace std;

struct CommandResult {
    std::string output;
    int         return_code;

    CommandResult() : output(), return_code() {}
    CommandResult(int i) : output(), return_code(i) {}
    CommandResult(std::string s) : output(s), return_code() {}
    CommandResult(std::string s, int i) : output(s), return_code(i) {}
    CommandResult(std::pair<std::string, int> p)
        : output(p.first), return_code(p.second) {}
};

template <typename T>
struct parse_string {
    T operator()(string value) {
        stringstream ss(value);
        T            ret;
        ss >> ret;
        return ret;
    }
};
enum class enabler_t {};

template <typename T>
using enableif = typename std::enable_if<T::value, enabler_t>::type;

namespace detail {
    template <typename... Args, typename T,
              typename std::enable_if<!std::is_same<T, void>::value, enabler_t>::type...>
    auto make_command(function<T(Args...)> f) {
        return [=](vector<string> s) {
            return CommandResult(::apply_to_fun(f, ::to_tuple<parse_string, Args...>(s)));
        };
    }

    template <typename... Args, typename T, enableif<std::is_same<T, void>>...>
    auto make_command(function<T(Args...)> f) {
        return [=](vector<string> s) {
				::apply_to_fun(f, ::to_tuple<parse_string, Args...>(s));
			return CommandResult();
        };
    }
}

template <typename T>
std::function<CommandResult(std::vector<std::string>)> make_command(T t) {
    return detail::make_command(make_function(t));
}

typedef std::function<CommandResult(std::vector<std::string>)> Command;

/*
 * struct Command {
 *     Command() {
 *         make_command(no_arg)({{"1"}, {"1.4"}});
 *         cout << make_command(test_fun)({{"1"}, {"1.4"}}) << endl;
 *     }
 * };
 */

#endif
