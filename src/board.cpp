#include "board.h"

using namespace std;

Board::Board(){
    printf("YOOOOOOO\n");
}

Board Board::get_board(){
    printf("Board got\n");
    return Board();
}

Board Board::get_board_from_file(string filepath){
    printf("Board got from file %s\n", filepath.c_str());
    return Board();
}

void Board::set_board(){
    printf("Board set\n");
}

