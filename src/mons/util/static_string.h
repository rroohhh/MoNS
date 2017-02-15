/*
  Copyright (C) 2017 by Robin Heinemann. All Rights Reserved.
  static_string.h -- compile time static string
*/

#ifndef _STATIC_STRING_H
#define _STATIC_STRING_H

#include <ostream>
#include <cstdint>
#include <string>
#include <stdexcept>

class StaticString {
private:
    const char * const  d;
    const std::uint64_t s;

public:
    using const_iterator = const char *;

    template <std::uint64_t n>
    constexpr StaticString(const char (&string)[n]) noexcept
        : d(string), s(n - 1) {}

    constexpr StaticString(const char * string, std::uint64_t n) noexcept
        : d(string), s(n) {}

    constexpr const char * data() const noexcept { return d; }
    constexpr std::uint64_t     size() const noexcept { return s; }

    constexpr const_iterator begin() const noexcept { return d; }
    constexpr const_iterator end() const noexcept { return d + s; }

    constexpr char operator[](std::uint64_t n) const {
        return n < s ? d[n] : throw std::out_of_range("static_string");
    }

    std::string toStdString() { return std::string(d, s); }
};

inline std::ostream & operator<<(std::ostream & os, StaticString const & s) {
    return os.write(s.data(), s.size());
}

#endif
