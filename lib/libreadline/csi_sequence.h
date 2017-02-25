/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  csi_sequence.h -- CSI sequence handling
*/

#ifndef __CSI_SEQUENCE_H_
#define __CSI_SEQUENCE_H_

#include "ascii.h"
#include <cstdio>
#include <memory>

namespace csi_sequence {
    // NOTE(robin): this is a shitty hash :), but works
    constexpr short encode_escape_sequence(const char c) noexcept {
        return c + 1000;
    }

    constexpr char decode_escape_sequence(const short c) noexcept {
        return c - 1000;
    }

    constexpr short unhandled        = encode_escape_sequence(0);
    constexpr short arrow_up         = encode_escape_sequence('A');
    constexpr short arrow_down       = encode_escape_sequence('B');
    constexpr short arrow_right      = encode_escape_sequence('C');
    constexpr short arrow_left       = encode_escape_sequence('D');
    constexpr int   MAX_ESC_SEQ_ARGS = 4;

    static short sequence_table[128][2] = {
        {0, unhandled},   {1, unhandled},   {2, unhandled},   {3, unhandled},
        {4, unhandled},   {5, unhandled},   {6, unhandled},   {7, unhandled},
        {8, unhandled},   {9, unhandled},   {10, unhandled},  {11, unhandled},
        {12, unhandled},  {13, unhandled},  {14, unhandled},  {15, unhandled},
        {16, unhandled},  {17, unhandled},  {18, unhandled},  {19, unhandled},
        {20, unhandled},  {21, unhandled},  {22, unhandled},  {23, unhandled},
        {24, unhandled},  {25, unhandled},  {26, unhandled},  {27, unhandled},
        {28, unhandled},  {29, unhandled},  {30, unhandled},  {31, unhandled},
        {32, unhandled},  {33, unhandled},  {34, unhandled},  {35, unhandled},
        {36, unhandled},  {37, unhandled},  {38, unhandled},  {39, unhandled},
        {40, unhandled},  {41, unhandled},  {42, unhandled},  {43, unhandled},
        {44, unhandled},  {45, unhandled},  {46, unhandled},  {47, unhandled},
        {48, unhandled},  {49, unhandled},  {50, unhandled},  {51, unhandled},
        {52, unhandled},  {53, unhandled},  {54, unhandled},  {55, unhandled},
        {56, unhandled},  {57, unhandled},  {58, unhandled},  {59, unhandled},
        {60, unhandled},  {61, unhandled},  {62, unhandled},  {63, unhandled},
        {64, unhandled},  {65, arrow_up},   {66, arrow_down}, {67, arrow_right},
        {68, arrow_left}, {69, unhandled},  {70, unhandled},  {71, unhandled},
        {72, unhandled},  {73, unhandled},  {74, unhandled},  {75, unhandled},
        {76, unhandled},  {77, unhandled},  {78, unhandled},  {79, unhandled},
        {80, unhandled},  {81, unhandled},  {82, unhandled},  {83, unhandled},
        {84, unhandled},  {85, unhandled},  {86, unhandled},  {87, unhandled},
        {88, unhandled},  {89, unhandled},  {90, unhandled},  {91, unhandled},
        {92, unhandled},  {93, unhandled},  {94, unhandled},  {95, unhandled},
        {96, unhandled},  {97, unhandled},  {98, unhandled},  {99, unhandled},
        {100, unhandled}, {101, unhandled}, {102, unhandled}, {103, unhandled},
        {104, unhandled}, {105, unhandled}, {106, unhandled}, {107, unhandled},
        {108, unhandled}, {109, unhandled}, {110, unhandled}, {111, unhandled},
        {112, unhandled}, {113, unhandled}, {114, unhandled}, {115, unhandled},
        {116, unhandled}, {117, unhandled}, {118, unhandled}, {119, unhandled},
        {120, unhandled}, {121, unhandled}, {122, unhandled}, {123, unhandled},
        {124, unhandled}, {125, unhandled}, {126, unhandled}, {127, unhandled},
    };

#define declare_sequence(x, ...) const char x[] = {__VA_ARGS__}

    declare_sequence(clear_line, ascii::ESC, ascii::SQUARE_BRACKET_OPEN,
                     ascii::TWO, ascii::K);
    declare_sequence(beginning_of_line, ascii::CR);

    inline std::pair<std::unique_ptr<char[]>, int> goto_x(int x) noexcept {
        if(x > 0) {
            int len = snprintf(nullptr, 0, "%c%c%c%d%c", ascii::CR, ascii::ESC,
                               ascii::SQUARE_BRACKET_OPEN, x, ascii::C) + 1;
            std::unique_ptr<char[]> str(new char[len]);

            snprintf(str.get(), len, "%c%c%c%d%c", ascii::CR, ascii::ESC,
                     ascii::SQUARE_BRACKET_OPEN, x, ascii::C);

            return std::make_pair(std::move(str), len);
        } 
            int                     len = snprintf(nullptr, 0, "\r") + 1;
            std::unique_ptr<char[]> str(new char[len]);

            snprintf(str.get(), len, "\r");

            return std::make_pair(std::move(str), len);
        
    }

    inline short char_to_type(const char c) noexcept {
        return sequence_table[(int)c][1];
    }

    inline char type_to_char(const short s) noexcept {
        return decode_escape_sequence(s);
    }

    struct esc_seq {
        short type;
        int   arguments[MAX_ESC_SEQ_ARGS];
    };
}

#endif
