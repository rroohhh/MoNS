// Robin Heinemann Copyright (C) 2015

#include "axis_scale.h"

int axis_scale::get_optimal_exponent(double low, double high) {
    double range          = high - low;
    int    range_exponent = floor(log10(range));

    int count = (int)floor(fabs(range / pow10(range_exponent)));

    return ((count >= 3) ? range_exponent : (range_exponent - 1));
}
