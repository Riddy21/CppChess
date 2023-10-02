#ifdef SWIG

%module board
%{
#include "board.h"
%}

%include <std_vector.i>
%template(COORD) std::vector<int>;

#endif
#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <map>
#include <vector>
#include <stdio.h>

#include "settings.h"

using namespace std;

typedef vector<int> COORD;

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
