#ifdef SWIG

%module board
%{
#include "board.h"
%}

%include "settings.h";
%include "piece.h"
%include <std_vector.i>;
%include <std_pair.i>;

%template(BOARD_PIECES) std::vector<Piece *>;
%template(BOARD_COORDS) std::vector<COORD>;
%template(BOARD_ITEM) std::pair<COORD, Piece *>;
%template(BOARD_ITEMS) std::vector<std::pair<COORD, Piece *>>;

#endif

#ifndef BOARD_H
#define BOARD_H
#pragma once // Make sure it's compiled only once

#include "settings.h"
#include "piece.h"

using namespace std;

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
    Piece * get(COORD coord) const;

    /**
     * @brief set the board object by coord
     * 
     * @return Piece* 
     */
    void set(COORD coord, Piece * piece);

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
     * @brief Gets a list of all the pieces in board
     * 
     * @return vector<Piece *> 
     */
    const vector<Piece *> pieces() const;

    /**
     * @brief Gets the list of all coordinates of pieces
     * 
     * @return const vector<COORD> 
     */
    const vector<COORD> coords() const;

    /**
     * @brief Returns the pair of coordinate and items from the board
     * 
     * @return const vector<pair<COORD, Piece *>> 
     */
    const vector<pair<COORD, Piece *>> items() const;

    /**
     * @brief Convert to string
     * 
     * @return const char* 
     */
    const char * to_str() const;

    /**
     * @brief Convert to num moves string
     * 
     * @return const char*
     */
    const char * to_num_moves() const;

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
    Piece * __getitem__(COORD coord) const { return get(coord); }

    /**
     * @brief Python dictionary functions setting dict item
     * 
     * @param coord 
     */
    void __setitem__(COORD coord, Piece * piece) { set(coord, piece); }



private:
    static Piece * BLANK_PIECE;
    unordered_map<COORD, Piece *, ArrayHash> board_map;

    static bool is_out_of_bounds(COORD coord);

};

#endif
