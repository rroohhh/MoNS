/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  Command.h -- wraps one command
*/

#ifndef __COMMAND_H_
#define __COMMAND_H_

#include "types.h"
#include "util.h"

namespace mons {
    struct command_result {
        string output;
        int    return_code;

        command_result() : output(), return_code() {}
        command_result(int i) : output(), return_code(i) {}
        command_result(string s) : output(move(s)), return_code() {}
        command_result(string s, int i) : output(move(s)), return_code(i) {}
        command_result(pair<string, int> p)
            : output(p.first), return_code(p.second) {}
    };

    template <typename T>
    struct parse_string {
        T operator()(const string & value) {
            stringstream ss(value);
            T            ret;
            ss >> ret;
            return ret;
        }
    };
    enum class enabler_t {};

    template <typename T>
    using enableif = typename enable_if<T::value, enabler_t>::type;

    namespace detail {
#if !defined(__INTEL_COMPILER)
        template <
            typename... Args, typename T,
            typename enable_if<!is_same<T, void>::value, enabler_t>::type...>
        auto make_command(function<T(Args...)> f) {
            return [=](vector<string> s) {
                return command_result(mons::apply_to_fun(
                    f, mons::to_tuple<parse_string, Args...>(s)));
            };
        }

        template <typename... Args, typename T, enableif<is_same<T, void>>...>
        auto make_command(function<T(Args...)> f) {
            return [=](vector<string> s) {
                mons::apply_to_fun(f, mons::to_tuple<parse_string, Args...>(s));
                return command_result();
            };
        }
#else
        template <typename... Args, typename T>
        auto make_command(function<T(Args...)> f) {
            return [=](vector<string> s) {
                return command_result(mons::apply_to_fun(
                    f, mons::to_tuple<parse_string, Args...>(s)));
            };
        }
#endif
    }

    template <typename T>
    function<command_result(vector<string>)> make_command(T t) {
        return detail::make_command(make_function(t));
    }

    using command = function<command_result(vector<string>)>;
}

/*
 * struct Command {
 *     Command() {
 *         make_command(no_arg)({{"1"}, {"1.4"}});
 *         cout << make_command(test_fun)({{"1"}, {"1.4"}}) << endl;
 *     }
 * };
 */

#endif
