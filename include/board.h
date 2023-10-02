#ifdef SWIG

%module board
%{
#include "board.h"
%}

%include "settings.h"

#endif
#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <map>
#include <stdio.h>

#include "settings.h"

using namespace std;

class Board {
    struct Square {
        unsigned num_moves;
        // TODO: Add piece class here
    };
    public:
        Board();
        static Board * get_board_from_file(char * filepath);
        static void set_board(Board * board, char * filepath);
        const char * __str__();

    private:
        map<string, Square> board;
};

#endif
