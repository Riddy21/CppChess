#include "move.h"

Move::~Move() {
    // Delete the captured piece pointer
    delete captured_piece;
    delete promo_piece;
}

Move * Move::make_move(COORD source, COORD target, Board * board, TYPE promotion_type) {
    Movesets::MOVE_TYPE move_type = Movesets::get_move_type(source, target, board);
    if (move_type == Movesets::INVALID)
        throw runtime_error("Invalid move");

    // Make sure promo type is valid
    if (promotion_type != QUEEN && promotion_type != ROOK && promotion_type != BISHOP && promotion_type != KNIGHT)
        throw runtime_error("Invalid promotion type " + to_string(promotion_type));

    // Make the move
    Piece * captured = nullptr;
    Piece * promo = nullptr;
    switch(move_type){
        case Movesets::MOVE:
            board->set(target, board->get(source));
            board->remove(source);
            board->get(target)->num_moves ++;
            break;
        case Movesets::CAPTURE:
            captured = board->get(target);
            board->set(target, board->get(source));
            board->remove(source);
            board->get(target)->num_moves ++;
            break;
        case Movesets::ENPASSANTE:
            captured = board->get({target[0], source[1]}); // The captured piece is the one behind the pawn
            board->remove({target[0], source[1]});
            board->set(target, board->get(source));
            board->remove(source);
            board->get(target)->num_moves ++;
            break;
        case Movesets::PROMOTION:
            promo = board->get(source); // Keep the previous pawn as promo piece
            // If the promotion was a capture, capture the piece
            if (board->get(target)->type != BLANK){
                captured = board->get(target);
                board->remove(target);
            }
            board->set(target, new Piece(board->get(source)->color, promotion_type)); // Set the new piece
            board->remove(source);
            board->get(target)->num_moves = promo->num_moves + 1; // Set the number of moves to the number of moves of the pawn + 1
            break;
        case Movesets::LCASTLE:
            board->set(target, board->get(source));
            board->remove(source);
            board->set({target[0]+1, target[1]}, board->get({target[0]-2, target[1]}));
            board->remove({target[0]-2, target[1]});
            board->get(target)->num_moves ++;
            board->get({target[0]+1, target[1]})->num_moves ++;
            break;
        case Movesets::RCASTLE:
            board->set(target, board->get(source));
            board->remove(source);
            board->set({target[0]-1, target[1]}, board->get({target[0]+1, target[1]}));
            board->remove({target[0]+1, target[1]});
            board->get(target)->num_moves ++;
            board->get({target[0]-1, target[1]})->num_moves ++;
            break;
        default:
            throw invalid_argument("Move type " + to_string(move_type) + " not implemented");
    }
    return new Move(source, target, move_type, captured, promo);
}

void Move::undo_move(Board * board) {
    switch(type){
        case Movesets::MOVE:
            board->set(source, board->get(target));
            board->remove(target);
            board->get(source)->num_moves --;
            break;
        case Movesets::CAPTURE:
            board->set(source, board->get(target));
            board->set(target, captured_piece);
            board->get(source)->num_moves --;
            break;
        case Movesets::ENPASSANTE:
            board->set(source, board->get(target));
            board->set({target[0], source[1]}, captured_piece);
            board->remove(target);
            board->get(source)->num_moves --;
            break;
        case Movesets::PROMOTION:
            board->set(source, promo_piece); // Set the pawn back
            delete board->get(target); // Delete the promoted piece
            board->remove(target); // Remove the promoted piece

            // if there was a captured piece, replace it
            if (captured_piece != nullptr)
                board->set(target, captured_piece);
            break;
        case Movesets::LCASTLE:
            board->set(source, board->get(target));
            board->set({target[0]-2, target[1]}, board->get({target[0]+1, target[1]}));
            board->remove({target[0]+1, target[1]});
            board->remove(target);
            board->get(source)->num_moves --;
            board->get({target[0]-2, target[1]})->num_moves --;
            break;
        case Movesets::RCASTLE:
            board->set(source, board->get(target));
            board->set({target[0]+1, target[1]}, board->get({target[0]-1, target[1]}));
            board->remove({target[0]-1, target[1]});
            board->remove(target);
            board->get(source)->num_moves --;
            board->get({target[0]+1, target[1]})->num_moves --;
            break;
        default:
            throw invalid_argument("Move type " + to_string(type) + " not implemented");
    }
}

const char * Move::__str__() const {
    string output = "";

    output += "Type: " + to_string(type) + "\n";
    // Get the source and target
    output += "Source: " + to_string(source[0]) + ", " + to_string(source[1]) + "\n";
    output += "Target: " + to_string(target[0]) + ", " + to_string(target[1]) + "\n";

    // Get the captured piece
    if (captured_piece != nullptr)
        output += "Captured piece: " + string(1, *captured_piece->__str__()) + "\n";

    char * c_str_out = new char[output.length()+1];
    strcpy(c_str_out, output.c_str());
    return c_str_out;
}

Move::Move(COORD source, COORD target, Movesets::MOVE_TYPE type, Piece * captured_piece, Piece * promo_piece) {
    this->source = source;
    this->target = target;
    this->type = type;
    this->captured_piece = captured_piece;
    this->promo_piece = promo_piece;
}