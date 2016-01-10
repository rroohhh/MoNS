// Robin Heinemann Copyright (C) 2015

#include "AxisScale.h"

int AxisScale::getOptimalExponent(double low, double high)
{
	double range = high - low;
	int rangeExponent = floor(log10(range));
	
	int countU = (int) floor(fabs(range / pow10(rangeExponent)));

	return ((countU >= 3) ? rangeExponent : (rangeExponent - 1)); 
}
