/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  ascii.h -- ascii symbols and helpers
*/

#ifndef _ASCII_H
#define _ASCII_H

#undef ESC
#undef SPACE

namespace ascii {
	constexpr int NUL                  = 0;
	constexpr int SOH                  = 1;
	constexpr int STX                  = 2;
	constexpr int ETX                  = 3;
	constexpr int EOT                  = 4;
	constexpr int ENQ                  = 5;
	constexpr int ACK                  = 6;
	constexpr int BEL                  = 7;
	constexpr int BS                   = 8;
	constexpr int HT                   = 9;
	constexpr int LF                   = 10;
	constexpr int VT                   = 11;
	constexpr int FF                   = 12;
	constexpr int CR                   = 13;
	constexpr int SO                   = 14;
	constexpr int SI                   = 15;
	constexpr int DLE                  = 16;
	constexpr int DC1                  = 17;
	constexpr int DC2                  = 18;
	constexpr int DC3                  = 19;
	constexpr int DC4                  = 20;
	constexpr int NAK                  = 21;
	constexpr int SYN                  = 22;
	constexpr int ETB                  = 23;
	constexpr int CAN                  = 24;
	constexpr int EM                   = 25;
	constexpr int SUB                  = 26;
	constexpr int ESC                  = 27;
	constexpr int FS                   = 28;
	constexpr int GS                   = 29;
	constexpr int RS                   = 30;
	constexpr int US                   = 31;
	constexpr int SPACE                = 32;
	constexpr int EXCLAMATION_MARK     = 33;
	constexpr int QUOTATION_MARK       = 34;
	constexpr int NUMBER_SIGN          = 35;
	constexpr int DOLLAR_SIGN          = 36;
	constexpr int PERCENT_SIGN         = 37;
	constexpr int AMPERSAND            = 38;
	constexpr int APOSTROPHE           = 39;
	constexpr int ROUND_BRACKET_OPEN   = 40;
	constexpr int ROUND_BRACKET_CLOSE  = 41;
	constexpr int ASTERISK             = 42;
	constexpr int PLUS_SIGN            = 43;
	constexpr int COMMA                = 44;
	constexpr int HYPHEN               = 45;
	constexpr int DOT                  = 46;
	constexpr int SLASH                = 47;
	constexpr int ZERO                 = 48;
	constexpr int ONE                  = 49;
	constexpr int TWO                  = 50;
	constexpr int THREE                = 51;
	constexpr int FOUR                 = 52;
	constexpr int FIVE                 = 53;
	constexpr int SIX                  = 54;
	constexpr int SEVEN                = 55;
	constexpr int EIGHT                = 56;
	constexpr int NINE                 = 57;
	constexpr int COLON                = 58;
	constexpr int SEMICOLON            = 59;
	constexpr int LESS_THAN            = 60;
	constexpr int EQUALS_SIGN          = 61;
	constexpr int GREATER_SIGN         = 62;
	constexpr int QUESTION_MARK        = 63;
	constexpr int AT_SIGN              = 64;
	constexpr int A                    = 65;
	constexpr int B                    = 66;
	constexpr int C                    = 67;
	constexpr int D                    = 68;
	constexpr int E                    = 69;
	constexpr int F                    = 70;
	constexpr int G                    = 71;
	constexpr int H                    = 72;
	constexpr int I                    = 73;
	constexpr int J                    = 74;
	constexpr int K                    = 75;
	constexpr int L                    = 76;
	constexpr int M                    = 77;
	constexpr int N                    = 78;
	constexpr int O                    = 79;
	constexpr int P                    = 80;
	constexpr int Q                    = 81;
	constexpr int R                    = 82;
	constexpr int S                    = 83;
	constexpr int T                    = 84;
	constexpr int U                    = 85;
	constexpr int V                    = 86;
	constexpr int W                    = 87;
	constexpr int X                    = 88;
	constexpr int Y                    = 89;
	constexpr int Z                    = 90;
	constexpr int SQUARE_BRACKET_OPEN  = 91;
	constexpr int BACKSLASH            = 92;
	constexpr int SQUARE_BRACKET_CLOSE = 93;
	constexpr int CARET                = 94;
	constexpr int UNDERSCORE           = 95;
	constexpr int GRAVE                = 96;
	constexpr int a                    = 97;
	constexpr int b                    = 98;
	constexpr int c                    = 99;
	constexpr int d                    = 100;
	constexpr int e                    = 101;
	constexpr int f                    = 102;
	constexpr int g                    = 103;
	constexpr int h                    = 104;
	constexpr int i                    = 105;
	constexpr int j                    = 106;
	constexpr int k                    = 107;
	constexpr int l                    = 108;
	constexpr int m                    = 109;
	constexpr int n                    = 110;
	constexpr int o                    = 111;
	constexpr int p                    = 112;
	constexpr int q                    = 113;
	constexpr int r                    = 114;
	constexpr int s                    = 115;
	constexpr int t                    = 116;
	constexpr int u                    = 117;
	constexpr int v                    = 118;
	constexpr int w                    = 119;
	constexpr int x                    = 120;
	constexpr int y                    = 121;
	constexpr int z                    = 122;
	constexpr int CURLY_BRACKET_OPEN   = 123;
	constexpr int VERTICAL_SLASH       = 124;
	constexpr int CURLY_BRACKET_CLOSE  = 125;
	constexpr int TILDE                = 126;
	constexpr int DEL                  = 127;
}

#endif
