#include "move.h"

Move * Move::make_move(COORD source, COORD target, Board * board) {
    Movesets::MOVE_TYPE move_type = Movesets::get_move_type(source, target, board);
    if (move_type == Movesets::INVALID)
        throw runtime_error("Invalid move");

    // Make the move
    const Piece * temp;
    switch(move_type){
        case Movesets::MOVE:
            board->set(target, board->get(source));
            board->remove(source);
            break;
        case Movesets::CAPTURE:
        case Movesets::ENPASSANTE:
        case Movesets::PROMOTION:
        case Movesets::LCASTLE:
        case Movesets::RCASTLE:
        default:
            throw invalid_argument("Move type " + to_string(move_type) + " not implemented");
    }
}

void Move::undo_move(Board * board) {

}

const char * Move::__str__() const {

}

Move::Move(COORD source, COORD target, Movesets::MOVE_TYPE type) {

}