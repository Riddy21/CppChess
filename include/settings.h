#ifdef SWIG

%module settings
%{
#include "settings.h"
%}

%include <std_array.i>;
%include <std_except.i>;
%include <std_unordered_map.i>;

%template(COORD) std::array<unsigned, 2>;
%template(MOVE_COORDS) std::array<std::array<unsigned, 2>, 2>;
%apply char { TYPE };

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

enum TYPE{
    PAWN = 'P',
    KING = 'K',
    QUEEN = 'Q',
    BISHOP = 'B',
    KNIGHT = 'N',
    ROOK = 'R',
    BLANK = '-',
};

constexpr unsigned get_value(TYPE piece) {
    switch (piece) {
        case BLANK: return 0;
        case PAWN: return 1;
        case KNIGHT: return 3;
        case BISHOP: return 3;
        case ROOK: return 5;
        case QUEEN: return 9;
        case KING: return INT_MAX;
        default: throw std::logic_error("Piece " + std::string(1, piece) + " not found.") ;  // Handle unknown piece types
    }
}
 
#endif
