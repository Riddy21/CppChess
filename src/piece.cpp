#include "piece.h"

using namespace std;

Piece::Piece(COLOR color, TYPE type) {
    this->color = color;
    this->num_moves = 0;
    this->type = type;
    this->value = get_value(type);
}

const char * Piece::__str__() const {
    char * output = new char[2];
    if (this->color == WHITE)
        output[0] = toupper(type);
    else
        output[0] = tolower(type);

    output[1] = '\0';

    return output;
}
