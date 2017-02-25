/*
  Copyright (C) 2017 by Robin Heinemann. All Rights Reserved.
  split.h -- split string by delimiter
*/

#ifndef __SPLIT_H_
#define __SPLIT_H_

#include "../types.h"

namespace mons {
    inline vector<string> split(const string & value, char separator) {
        vector<string>    result;
        string::size_type p = 0;
        string::size_type q;

        while((q = value.find(separator, p)) != string::npos) {
            result.emplace_back(value, p, q - p);
            p = q + 1;
        }
        result.emplace_back(value, p);
        return result;
    }

    inline vector<string> split(const string value) {
        istringstream iss{value};
        return {istream_iterator<string>{iss}, istream_iterator<string>{}};
    }
}

#endif
