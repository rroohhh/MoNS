/*
  Copyright (C) 2017 by Robin Heinemann. All Rights Reserved.
  split.h -- split string by delimiter
*/

#ifndef _SPLIT_H
#define _SPLIT_H

#include <string>
#include <vector>

inline std::vector<std::string> split(const std::string & value, char separator) {
    std::vector<std::string> result;
    std::string::size_type   p = 0;
    std::string::size_type   q;
    while((q = value.find(separator, p)) != std::string::npos) {
        result.emplace_back(value, p, q - p);
        p = q + 1;
    }
    result.emplace_back(value, p);
    return result;
}

#endif
