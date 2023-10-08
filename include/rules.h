#ifdef SWIG

%module rules
%{
#include "rules.h"
%}
%include "settings.h"
%include "board.h"

#endif
#ifndef RULES_H
#define RULES_H
#pragma once // Make sure it's compiled only once

#include <stdio.h>

#include "settings.h"
#include "board.h"

namespace Rules {
    enum OBSTRUCT_TYPE{
        SELF,
        OPPONENT,
        OPEN,
        OUT_OF_BOUNDS,
    };

    /**
     * @brief Detect if the coordinate is out of bounds
     * 
     * @param coord 
     * @return true 
     * @return false 
     */
    bool is_out_of_bounds(COORD coord);

    /**
     * @brief Detect if the piece is obstructed
     * @param source Where the piece is moving from
     * @param target Where the piece is intending to move to
     * @param board The board where the piece is moving
     * 
     * @return OBSTRUCT_TYPE
     */
    OBSTRUCT_TYPE detect_obstruction(COORD source, COORD target, Board * board);

    /**
     * @brief Get the pawn moves from the given board
     * 
     * @param source 
     * @param board 
     * @return MOVESET 
     */
    MOVESET get_pawn_moves(COORD source, Board * board);

};

#endif