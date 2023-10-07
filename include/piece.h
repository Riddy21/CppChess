#ifdef SWIG

%module piece
%{
#include "piece.h"
%}

%include "settings.h";

#endif
#ifndef PIECE_H
#define PIECE_H
#pragma once // Make sure it's compiled only once

#include "settings.h"

using namespace std;

class Piece {
    public:
        COLOR color;
        unsigned num_moves = 0;
        TYPE type;
        unsigned value;
        /**
         * @brief Construct a new Piece object
         * 
         * @param color 
         * @param type 
         */
        Piece(COLOR color, TYPE type);

        /**
         * @brief Converts the board to string for python
         * 
         * @return const char* 
         */
        const char* __str__() const;
};
#endif
