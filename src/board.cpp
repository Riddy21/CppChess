#include "board.h"

using namespace std;

Board::Board(){
    // Nothing to initialize
    return;
}

Board * Board::get_board_from_file(char * filepath){
    Board * new_board = new Board();
    set_board(new_board, filepath);
    return new_board;
}

void Board::set_board(Board * board, char * filepath){
    // TODO: To be implemented
}

const char * Board::__str__(){
    // TODO: To be implemented
    return (char *) "Board is printed";
}

