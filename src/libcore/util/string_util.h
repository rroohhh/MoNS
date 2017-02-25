/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  string_util.h -- string utilities
*/

#ifndef __STRING_UTIL_H_
#define __STRING_UTIL_H_

#include "../types.h"

namespace mons {
    static inline string & ltrim(string & s) noexcept {
        s.erase(s.begin(),
                find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
        return s;
    }

    static inline string & rtrim(string & s) noexcept {
        s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace)))
                    .base(),
                s.end());
        return s;
    }

    static inline string & trim(string & s) noexcept { return ltrim(rtrim(s)); }

    static inline string trimmed(string s) noexcept { return ltrim(rtrim(s)); }

    static inline string ltrimmed(string s) noexcept { return ltrim(s); }

    static inline string rtrimmed(string s) noexcept { return rtrim(s); }

    static inline string & surround_by(string & s,
                                       string & surrounding) noexcept {
        s.insert(0, surrounding);
        s.append(surrounding);

        return s;
    }

    template <typename T>
    static inline string str(T t) noexcept {
        ostringstream strs;
        strs << t;
        return strs.str();
    }
}

#endif
