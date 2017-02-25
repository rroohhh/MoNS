/*
  Copyright (C) 2017 by Robin Heinemann. All Rights Reserved.
  static_string.h -- compile time static string
*/

#ifndef __STATIC_STRING_H_
#define __STATIC_STRING_H_

#include "../types.h"

namespace mons {
    class static_string {
    private:
        const char * const d;
        const u64          s;

    public:
        using const_iterator = const char *;

        template <u64 n>
        constexpr static_string(const char (&string)[n]) noexcept
            : d(string), s(n - 1) {}

        constexpr static_string(const char * string, u64 n) noexcept
            : d(string), s(n) {}

        constexpr const char * data() const noexcept { return d; }
        constexpr u64          size() const noexcept { return s; }

        constexpr const_iterator begin() const noexcept { return d; }
        constexpr const_iterator end() const noexcept { return d + s; }

        constexpr char operator[](u64 n) const {
            return n < s ? d[n] : throw out_of_range("static_string");
        }

        string toStdString() { return string(d, s); }
    };

    inline ostream & operator<<(ostream & os, static_string const & s) {
        return os.write(s.data(), s.size());
    }
}

#endif
