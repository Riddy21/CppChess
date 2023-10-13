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

private:
    COORD source;
    COORD target;
    Movesets::MOVE_TYPE type;
    Piece * captured_piece;

    /**
     * @brief Construct the move class, private contructor to be created by static methods
     * 
     * @param source
     * @param target
     * @param type
     */
    Move(COORD source, COORD target, Movesets::MOVE_TYPE type, Piece * captured_piece);

};

#endif