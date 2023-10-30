#ifdef SWIG

%module movesets
%{
#include "movesets.h"
%}
%include "settings.h"
%include "board.h"

%include <std_unordered_set.i>;
%include <std_vector.i>;

%template(MOVESET) std::unordered_set<std::array<unsigned, 2>, ArrayHash>;
%template(TYPE_VECTOR) std::vector<TYPE>;

#endif
#ifndef MOVESETS_H
#define MOVESETS_H
#pragma once // Make sure it's compiled only once

#include "settings.h"
#include "board.h"

typedef std::unordered_set<COORD, ArrayHash> MOVESET;

class Movesets {
public:
    enum MOVE_TYPE{
        MOVE,
        CAPTURE,
        LCASTLE,
        RCASTLE,
        ENPASSANTE,
        PROMOTION,
        INVALID,
    };

    /**
     * @brief Get the movetype of the given move
     * 
     * @param source
     * @param target
     * @param board
     * @return MOVETYPE
     */
    static MOVE_TYPE get_move_type(COORD source, COORD target, Board * board);

    /**
     * @brief Get moves given a source and target on the board
     * 
     * @param source
     * @param target
     * @param board
     * @return MOVESET
     */
    static MOVESET get_moves(COORD source, Board * board);

    /**
     * @brief Get all valid promotion types
     * 
     */
    static std::vector<TYPE> get_valid_promo_types();

    /**
     * @brief check if move is a pawn promotion
     * 
     * @param source
     * @param target
     * @param board
     * @return true
     * @return false
     */
    static bool is_pawn_promo(COORD source, COORD target, Board * board);

    /**
     * @brief check if move is a castle
     * 
     * @param source
     * @param target
     * @param board
     * @return true
     * @return false
     * 
     */
    static bool is_castle(COORD source, COORD target, Board * board);

private:
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
    static bool is_out_of_bounds(COORD coord);

    /**
     * @brief Detect if the piece is obstructed
     * @param source Where the piece is moving from
     * @param target Where the piece is intending to move to
     * @param board The board where the piece is moving
     * 
     * @return OBSTRUCT_TYPE
     */
    static OBSTRUCT_TYPE detect_obstruction(COORD source, COORD target, Board * board);

    /**
     * @brief Get the pawn moves from the given board
     * 
     * @param source 
     * @param board 
     * @return MOVESET 
     */
    static MOVESET get_pawn_moves(COORD source, Board * board);

    /**
     * @brief Get enpassante moves from the given board
     * 
     * @param source
     * @param board
     * @return MOVESET
     * 
     */
    static MOVESET get_enpassante_moves(COORD source, Board * board);

    /**
     * @brief Get knight moves from the given board
     * 
     * @param source
     * @param board
     * @return MOVESET
     * 
     */
    static MOVESET get_knight_moves(COORD source, Board * board);

    /**
     * @brief Get the left castle moves
     * 
     * @param source 
     * @param board 
     * @return MOVESET 
     */
    static MOVESET get_left_castle_moves(COORD source, Board * board);

    /**
     * @brief Get the right castle moves
     * 
     * @param source
     * @param board
     * @return MOVESET
     */
    static MOVESET get_right_castle_moves(COORD source, Board * board);

    /**
     * @brief  Get the diagonal moves from the given board
     * 
     * @param source
     * @param board
     * @param spread The number of squares to spread
     * @return MOVESET
     */
    static MOVESET get_diagonal_moves(COORD source, Board * board, unsigned spread=BOARD_WIDTH);
    /**
     * @brief Get orthogonal moves from the given board
     * 
     * @param source
     * @param board
     * @param spread The number of squares to spread
     * @return MOVESET
     */
    static MOVESET get_orthogonal_moves(COORD source, Board * board, unsigned spread=BOARD_WIDTH);
};

#endif