#ifdef SWIG

%module move
%{
#include "move.h"
%}
%include "settings.h"
%include "movesets.h"

#endif
#ifndef MOVE_H
#define MOVE_H
#pragma once // Make sure it's compiled only once

#include "settings.h"
#include "movesets.h"
#include "piece.h"
#include "board.h"

class Move {
public:
    const COLOR turn;
    const Movesets::MOVE_TYPE type;

    ~Move();
    /**
     * @brief Makes a move from source to target and then returns the move object associated
     * 
     * @param source 
     * @param target 
     * @param board 
     * @return Move* 
     */
    static Move * make_move(COORD source, COORD target, Board * board, TYPE promotion_type = QUEEN);

    /**
     * @brief Undos the move on the board
     * 
     * @param move 
     * @param board 
     * @return Move* 
     */
    void undo_move(Board * board);

    /**
     * @brief Prints the move
     * 
     * @return const char *
     */
    const char * __str__() const;

    /**
     * @brief prints the value of the piece that was captured
     *
     * @return unsigned int
     */
    unsigned get_captured_value() const;

    /**
     * @brief get the source coordinate
     * 
     * @return COORD
     */
    const COORD get_source() const { return source; }

    /**
     * @brief get the target coordinate
     * 
     * @return COORD
     */
    const COORD get_target() const { return target; }

private:
    Piece * captured_piece;
    Piece * promo_piece;
    const COORD source;
    const COORD target;

    /**
     * @brief Construct the move class, private contructor to be created by static methods
     * 
     * @param source
     * @param target
     * @param type
     */
    Move(COORD source, COORD target, Movesets::MOVE_TYPE type, Piece * captured_piece, Piece * promo_piece, COLOR turn) :
        turn(turn), type(type), captured_piece(captured_piece), promo_piece(promo_piece), source(source), target(target) {};

};

#endif
