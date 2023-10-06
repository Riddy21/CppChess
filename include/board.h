#ifdef SWIG

%module board
%{
#include "board.h"
%}

%include "settings.h"

#endif
#ifndef BOARD_H
#define BOARD_H
#pragma once // Make sure it's compiled only once

#include "settings.h"

using namespace std;

class Board {
public:
    struct Piece {
        COLOR color;
        unsigned num_moves = 0;
        PIECE type;
        unsigned value;

        Piece(COLOR color, PIECE type) {
            this->color = color;
            this->num_moves = 0;
            this->type = type;
            this->value = PIECE_VALUES.at(type);
        }
    };

    /**
     * @brief Construct a new Board object with a filepath
     * 
     * @param filepath 
     */
    Board(char * filepath);

    /**
     * @brief Default constructor for emtpy board, nothing to initialize
     * 
     */
    Board(){}

    /**
     * @brief Destroy the Board object and all its internal pieces
     * 
     */
    ~Board();

    /**
     * @brief Get the board from a filepath
     * 
     * @param filepath 
     * @return Board* 
     */
    static Board * get_board_from_file(char * filepath);

    /**
     * @brief Static function to create another copy of the board object
     * 
     * @param board board you are going to copy
     * @return Pointer to the new copied board
     */
    static Board * copy_board(Board * board);
    
    /**
     * @brief Set the board object in place from file
     * 
     * @param board board you want to set
     * @param filepath The filepath to the board
     */
    static void set_board(Board * board, char * filepath);

    /**
     * @brief Convert the board to a string for python
     * 
     * @return string of the board
     */
    const char* __str__();

private:
    unordered_map<COORD, Piece *, ArrayHash> board_map;
};

#endif
