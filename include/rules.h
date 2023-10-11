#ifdef SWIG

%module rules
%{
#include "rules.h"
%}
%include "settings.h"
%include "movesets.h"
%include "board.h"

#endif
#ifndef RULES_H
#define RULES_H
#pragma once // Make sure it's compiled only once

#include "settings.h"
#include "movesets.h"
#include "board.h"

class Rules {
public:
    enum GAME_STATE{
        NORMAL,
        CHECK,
        CHECKMATE,
        STALEMATE,
        PROMOTION,
    };
    /**
     * @brief Check if the king is in check
     * 
     * @param board 
     * @param color 
     * @return true 
     * @return false 
     */
    static bool is_in_check(COLOR color, Board * board);

    /**
     * @brief Gets the state of the game currently
     * 
     * @param board
     * @param color
     * @return GAME_STATE
     */
    static GAME_STATE get_game_state(COLOR color, Board * board);

    /**
     * @brief Get moves given a source on the board, applies limitations from check
     * 
     * @param source
     * @param board
     * @return MOVESET
     */
    static MOVESET get_moves(COORD source, Board * board);

private:
    /**
     * @brief Remove the pieces from poss_moves that put you in check
     * 
     * @param poss_moves 
     * @param source 
     * @param board 
     * @return MOVESET 
     */
    static MOVESET chk_limit_moves(COORD source, MOVESET poss_moves, Board * board);

    /**
     * @brief Get the coordinates of the king
     * 
     * @param color
     * @param board
     * @return COORD
     */
    static COORD get_king_coord(COLOR color, Board * board);
};
#endif
