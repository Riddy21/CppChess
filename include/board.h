#ifdef SWIG

%module board
%{
#include "board.h"
%}

%include "settings.h"
%include <std_vector.i>;

%template(PIECE_LIST) std::vector<const Piece *>;


#endif
#ifndef BOARD_H
#define BOARD_H
#pragma once // Make sure it's compiled only once

#include "settings.h"

using namespace std;

class Piece {
    public:
        COLOR color;
        unsigned num_moves = 0;
        PIECE type;
        unsigned value;
        /**
         * @brief Construct a new Piece object
         * 
         * @param color 
         * @param type 
         */
        Piece(COLOR color, PIECE type);

        /**
         * @brief Converts the board to string for python
         * 
         * @return const char* 
         */
        const char* __str__() const;
};

class Board {
public:
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
     * @brief function to create another copy of the board object
     *
     * @return Pointer to the new copied board
     */
    Board * copy() const;
    
    /**
     * @brief Set the board object from file
     * 
     * @param filepath The filepath to the board
     */
    void set_board(char * filepath);

    /**
     * @brief get the board object by coord
     * 
     * @return Piece* 
     */
    const Piece * get(COORD coord) const;

    /**
     * @brief set the board object by coord
     * 
     * @return Piece* 
     */
    void set(COORD coord, const Piece * piece);

    /**
     * @brief delete a piece on the board by coord
     * 
     */
    void remove(COORD coord);

    /**
     * @brief gets the num of pieces
     * 
     * @return unsigned 
     */
    unsigned size() const;

    /**
     * @brief Gets a list of all the items in the dict
     * 
     * @return vector<const Piece *> 
     */
    const vector<const Piece *> get_pieces() const;

    /**
     * @brief Convert to string
     * 
     * @return const char* 
     */
    const char * to_str() const;

    /**
     * @brief Convert the board to a string for python
     * 
     * @return string of the board
     */
    const char* __str__() const { return to_str(); }; 

    /**
     * @brief Python dictionary functions deleting item
     * 
     * @param coord 
     */
    void __delitem__(COORD coord) { remove(coord); }

    /**
     * @brief Python dictionary functions getting dict item
     * 
     * @param coord 
     */
    const Piece * __getitem__(COORD coord) const { return get(coord); }

    /**
     * @brief Python dictionary functions setting dict item
     * 
     * @param coord 
     */
    void __setitem__(COORD coord, const Piece * piece) { set(coord, piece); }



private:
    static const Piece * BLANK_PIECE;
    unordered_map<COORD, const Piece *, ArrayHash> board_map;

};

#endif
