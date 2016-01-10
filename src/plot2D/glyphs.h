// Robin Heinemann Copyright (C) 2015

#ifndef _GLYPHS_H
#define _GLYPHS_H


extern char _binary_0_bin_start[];
extern char _binary_1_bin_start[];
extern char _binary_2_bin_start[];
extern char _binary_3_bin_start[];
extern char _binary_4_bin_start[];
extern char _binary_5_bin_start[];
extern char _binary_6_bin_start[];
extern char _binary_7_bin_start[];
extern char _binary_8_bin_start[];
extern char _binary_9_bin_start[];
extern char _binary_e_bin_start[];
extern char _binary___bin_start[];
extern char _binary_minus_bin_start[];

namespace Glyphs {
    int width = 39;
    int height = 64;

	char * dot = _binary___bin_start;

	char * minus = _binary_minus_bin_start;

	char * zero = _binary_0_bin_start;
    
	char * one = _binary_1_bin_start;
    
	char * two = _binary_2_bin_start;

	char * three = _binary_3_bin_start;
    
	char * four = _binary_4_bin_start;

	char * five = _binary_5_bin_start;

	char * six = _binary_6_bin_start;

	char * seven = _binary_7_bin_start;

	char * eight = _binary_8_bin_start;

	char * nine = _binary_9_bin_start;

	char * e = _binary_e_bin_start;
}

#endif
