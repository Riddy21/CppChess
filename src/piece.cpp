#include "board.h"

using namespace std;

Piece::Piece(COLOR color, PIECE type) {
    this->color = color;
    this->num_moves = 0;
    this->type = type;
    this->value = PIECE_VALUES.at(type);
}

const char * Piece::__str__() const {
    char * output = new char;
    if (this->color == WHITE)
        output[0] = toupper(type);
    else
        output[0] = tolower(type);
    
    return output;
}
