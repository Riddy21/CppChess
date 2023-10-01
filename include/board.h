#ifdef SWIG

%module board
%{
#include "board.h"
%}

#endif
#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <map>
#include <stdio.h>

using namespace std;

class Board {
    struct Square {
        unsigned num_moves;
        // TODO: Add piece class here
    };
    public:
        Board();
        static Board get_board();
        static Board get_board_from_file(string filepath);
        static void set_board();

    private:
        map<string, Square> board;
};

#endif
