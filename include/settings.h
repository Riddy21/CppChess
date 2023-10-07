#ifdef SWIG

%module settings
%{
#include "settings.h"
%}

%include <std_array.i>;
%include <std_except.i>;

%template(COORD) std::array<unsigned, 2>;
%template(MOVE_COORDS) std::array<std::array<unsigned, 2>, 2>;
%apply char { PIECE };

// Catch all exceptions in C++ and put them as Runtime errors
%exception {
    try {
        $action
    } catch (const std::exception &e) {
        SWIG_exception(SWIG_RuntimeError, e.what());
    }
}

#endif
#ifndef SETTINGS_H
#define SETTINGS_H
#pragma once // Make sure it's compiled only once

#include <array>
#include <vector>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <utility>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cctype>
#include <cstring>
#include <stdio.h>
#include <limits.h>

#include "utils.h"

#define DEFAULT_BOARD_PRESETS_PATH "Presets/default.txt"
#define BOARD_HEIGHT 8
#define BOARD_WIDTH 8

typedef std::array<unsigned, 2> COORD;
typedef std::array<COORD, 2> MOVE_COORDS;

enum COLOR{
    WHITE=1,
    BLACK=0,
    NONE=2,
};

enum PIECE{
    PAWN = 'P',
    KING = 'K',
    QUEEN = 'Q',
    BISHOP = 'B',
    KNIGHT = 'N',
    ROOK = 'R',
    BLANK = '-',
};

static const std::unordered_map<PIECE, unsigned> PIECE_VALUES = {
    {BLANK, 0},
    {PAWN, 1},
    {KNIGHT, 3},
    {BISHOP, 3},
    {ROOK, 5},
    {QUEEN, 9},
    {KING, INT_MAX}, // INT_MAX is halfway from the largest number to avoid overflow
};
#endif
