#include "movesets.h"

using namespace std;

Movesets::MOVE_TYPE Movesets::get_move_type(COORD source, COORD target, Board * board){
    // Check if move is valid
    // In bounds
    if (is_out_of_bounds(source) || is_out_of_bounds(target))
        return INVALID;


    OBSTRUCT_TYPE obstruction = detect_obstruction(source, target, board);
    // check not capturing own piece
    if (obstruction == SELF)
        return INVALID;

    // Check is castle
    if (get_left_castle_moves(source, board).count(target))
        return LCASTLE;
    if (get_right_castle_moves(source, board).count(target))
        return RCASTLE;

    // Check is enpassante
    if (get_enpassante_moves(source, board).count(target))
        return ENPASSANTE;

    // Check is promotion
    if (is_pawn_promo(source, target, board))
        return PROMOTION;

    // Check is capture
    if (obstruction == OPPONENT)
        return CAPTURE;
    if (obstruction == OPEN)
        return MOVE;

    return INVALID;

}

MOVESET Movesets::get_moves(COORD source, Board * board){
    // Get the type of the piece
    TYPE type = board->get(source)->type;
    MOVESET poss_moves;
    MOVESET add_moves;
    // Get the moves
    switch(type){
        case PAWN:
            poss_moves = get_pawn_moves(source, board);
            add_moves = get_enpassante_moves(source, board);
            poss_moves.insert(add_moves.begin(), add_moves.end());
            break;
        case KNIGHT:
            poss_moves = get_knight_moves(source, board);
            break;
        case BISHOP:
            poss_moves = get_diagonal_moves(source, board);
            break;
        case ROOK:
            poss_moves = get_orthogonal_moves(source, board, BOARD_WIDTH);
            break;
        case QUEEN:
            poss_moves = get_diagonal_moves(source, board, BOARD_WIDTH);
            add_moves = get_orthogonal_moves(source, board, BOARD_WIDTH);
            poss_moves.insert(add_moves.begin(), add_moves.end());
            break;
        case KING:
            poss_moves = get_diagonal_moves(source, board, 1);
            add_moves = get_orthogonal_moves(source, board, 1);
            poss_moves.insert(add_moves.begin(), add_moves.end());
            add_moves = get_left_castle_moves(source, board);
            poss_moves.insert(add_moves.begin(), add_moves.end());
            add_moves = get_right_castle_moves(source, board);
            poss_moves.insert(add_moves.begin(), add_moves.end());
            break;
        default:
            throw invalid_argument("Invalid piece type");
    }

    return poss_moves;
}

bool Movesets::is_pawn_promo(COORD source, COORD target, Board * board){
    if (board->get(source)->type == PAWN){
        if (board->get(source)->color == BLACK){
            if (target[1] == BOARD_HEIGHT-1)
                return true;
        } else if (board->get(source)->color == WHITE){
            if (target[1] == 0)
                return true;
        }
    }
    return false;
}

bool Movesets::is_out_of_bounds(COORD coord){
    return (coord[0] < 0 || coord[0] >= BOARD_WIDTH || coord[1] < 0 || coord[1] >= BOARD_HEIGHT);
}

Movesets::OBSTRUCT_TYPE Movesets::detect_obstruction(COORD source, COORD target, Board * board){
    if (is_out_of_bounds(target) || is_out_of_bounds(source))
        return OUT_OF_BOUNDS;

    // Make sure its not checking itself
    if (source != target){
        if (board->get(source)->type == BLANK)
            throw invalid_argument("Source cannot be blank");
        else if (board->get(target)->type == BLANK)
            return OPEN;
        else if (board->get(target)->color == board->get(source)->color)
            return SELF;
        else
            return OPPONENT;
    }
    throw invalid_argument("Source cannot equal target");
}

MOVESET Movesets::get_pawn_moves(COORD source, Board * board){
    unsigned x = source[0];
    unsigned y = source[1];
    MOVESET poss_moves;
    Movesets::OBSTRUCT_TYPE obstruct;

    if (board->get(source)->color == BLACK){
        unsigned i = 1;
        // Do move detection
        obstruct = detect_obstruction(source, {x, y+i}, board);
        while (i <= 2 && obstruct == OPEN){
            // Add the move
            poss_moves.insert({x, y+i});
            
            // No 2nd move on 2nd turn
            if (board->get(source)->num_moves >= 1)
                break;
            
            i++;
            obstruct = detect_obstruction(source, {x, y+i}, board);
        }

        // Sideways Capture
        obstruct = detect_obstruction(source, {x+1, y+1}, board);
        if (obstruct == OPPONENT)
            poss_moves.insert({x+1, y+1});
        obstruct = detect_obstruction(source, {x-1, y+1}, board);
        if (obstruct == OPPONENT)
            poss_moves.insert({x-1, y+1});
    } else if (board->get(source)->color == WHITE){
        unsigned i = 1;
        // Do move detection
        obstruct = detect_obstruction(source, {x, y-i}, board);
        while (i <= 2 && obstruct == OPEN){
            // Add the move
            poss_moves.insert({x, y-i});
            
            // No 2nd move on 2nd turn
            if (board->get(source)->num_moves >= 1)
                break;
            
            i++;
            obstruct = detect_obstruction(source, {x, y-i}, board);
        }

        // Sideways Capture
        obstruct = detect_obstruction(source, {x+1, y-1}, board);
        if (obstruct == OPPONENT)
            poss_moves.insert({x+1, y-1});
        obstruct = detect_obstruction(source, {x-1, y-1}, board);
        if (obstruct == OPPONENT)
            poss_moves.insert({x-1, y-1});
    }

    return poss_moves;
}

MOVESET Movesets::get_enpassante_moves(COORD source, Board * board){
    unsigned x = source[0];
    unsigned y = source[1];
    MOVESET poss_moves;

    COORD right = {x+1, y};
    COORD left = {x-1, y};

    if (y == 3 && board->get(source)->color == WHITE){
        // Right enpassante
        if (detect_obstruction(source, right, board) == OPPONENT)
            // Check if the piece is a pawn and has only moved once
            if (board->get(right)->type == PAWN && board->get(right)->num_moves == 1)
                // Check if the enpassante move is obstructed
                if (detect_obstruction(source, {x+1, y-1}, board) == OPEN)
                    poss_moves.insert({x+1, y-1});
        if (detect_obstruction(source, left, board) == OPPONENT)
            //Check if the pieces is a pawn and has only moved once
            if (board->get(left)->type == PAWN && board->get(left)->num_moves == 1)
                // Check if the enpassante move is obstructed
                if (detect_obstruction(source, {x-1, y-1}, board) == OPEN)
                    poss_moves.insert({x-1, y-1});
    } else if (y == 4 && board->get(source)->color == BLACK){
        // Left enpassante
        if (detect_obstruction(source, right, board) == OPPONENT)
            // Check if the piece is a pawn and has only moved once
            if (board->get(right)->type == PAWN && board->get(right)->num_moves == 1)
                // Check if the enpassante move is obstructed
                if (detect_obstruction(source, {x+1, y+1}, board) == OPEN)
                    poss_moves.insert({x+1, y+1});
        if (detect_obstruction(source, left, board) == OPPONENT)
            //Check if the pieces is a pawn and has only moved once
            if (board->get(left)->type == PAWN && board->get(left)->num_moves == 1)
                // Check if the enpassante move is obstructed
                if (detect_obstruction(source, {x-1, y+1}, board) == OPEN)
                    poss_moves.insert({x-1, y+1});
    }

    return poss_moves;
}

MOVESET Movesets::get_knight_moves(COORD source, Board * board){
    unsigned x = source[0];
    unsigned y = source[1];
    MOVESET poss_moves;
    Movesets::OBSTRUCT_TYPE obstruct;

    // Up
    obstruct = detect_obstruction(source, {x+1, y+2}, board);
    if (obstruct == OPEN || obstruct == OPPONENT)
        poss_moves.insert({x+1, y+2});
    obstruct = detect_obstruction(source, {x-1, y+2}, board);
    if (obstruct == OPEN || obstruct == OPPONENT)
        poss_moves.insert({x-1, y+2});

    // Down
    obstruct = detect_obstruction(source, {x+1, y-2}, board);
    if (obstruct == OPEN || obstruct == OPPONENT)
        poss_moves.insert({x+1, y-2});
    obstruct = detect_obstruction(source, {x-1, y-2}, board);
    if (obstruct == OPEN || obstruct == OPPONENT)
        poss_moves.insert({x-1, y-2});

    // Left
    obstruct = detect_obstruction(source, {x-2, y+1}, board);
    if (obstruct == OPEN || obstruct == OPPONENT)
        poss_moves.insert({x-2, y+1});
    obstruct = detect_obstruction(source, {x-2, y-1}, board);
    if (obstruct == OPEN || obstruct == OPPONENT)
        poss_moves.insert({x-2, y-1});

    // Right
    obstruct = detect_obstruction(source, {x+2, y+1}, board);
    if (obstruct == OPEN || obstruct == OPPONENT)
        poss_moves.insert({x+2, y+1});
    obstruct = detect_obstruction(source, {x+2, y-1}, board);
    if (obstruct == OPEN || obstruct == OPPONENT)
        poss_moves.insert({x+2, y-1});

    return poss_moves;
}

MOVESET Movesets::get_left_castle_moves(COORD source, Board * board){
    unsigned x = source[0];
    unsigned y = source[1];
    MOVESET poss_moves;
    Movesets::OBSTRUCT_TYPE obstruct;

    // Check if it is a king
    if (board->get(source)->type != KING)
        return poss_moves;

    // Check if the king has moved
    if (board->get(source)->num_moves != 0)
        return poss_moves;

    // Check if the rook has moved
    if (board->get({0, y})->num_moves != 0)
        return poss_moves;

    // Check if the spaces are obstructed
    obstruct = detect_obstruction(source, {x-1, y}, board);
    if (obstruct != OPEN)
        return poss_moves;
    obstruct = detect_obstruction(source, {x-2, y}, board);
    if (obstruct != OPEN)
        return poss_moves;
    obstruct = detect_obstruction(source, {x-3, y}, board);
    if (obstruct != OPEN)
        return poss_moves;

    // Add the move
    poss_moves.insert({x-2, y});

    return poss_moves;
}

MOVESET Movesets::get_right_castle_moves(COORD coord, Board * board){
    unsigned x = coord[0];
    unsigned y = coord[1];
    MOVESET poss_moves;
    Movesets::OBSTRUCT_TYPE obstruct;

    // Check if it is a king
    if (board->get(coord)->type != KING)
        return poss_moves;

    // Check if the king has moved
    if (board->get(coord)->num_moves != 0)
        return poss_moves;

    // Check if the rook has moved
    if (board->get({BOARD_WIDTH-1, y})->num_moves != 0)
        return poss_moves;

    // Check if the spaces are obstructed
    obstruct = detect_obstruction(coord, {x+1, y}, board);
    if (obstruct != OPEN)
        return poss_moves;
    obstruct = detect_obstruction(coord, {x+2, y}, board);
    if (obstruct != OPEN)
        return poss_moves;

    // Add the move
    poss_moves.insert({x+2, y});

    return poss_moves;
}

MOVESET Movesets::get_diagonal_moves(COORD coord, Board * board, unsigned spread){
    unsigned x = coord[0];
    unsigned y = coord[1];
    MOVESET poss_moves;

    Movesets::OBSTRUCT_TYPE obstruct;

    //Top left
    for (unsigned i=1; i<=spread; i++){
        obstruct = detect_obstruction(coord, {x-i, y-i}, board);
        if (obstruct == OPEN || obstruct == OPPONENT)
            poss_moves.insert({x-i, y-i});
        if (obstruct != OPEN)
            break;
    }

    //Top right
    for (unsigned i=1; i<=spread; i++){
        obstruct = detect_obstruction(coord, {x+i, y-i}, board);
        if (obstruct == OPEN || obstruct == OPPONENT)
            poss_moves.insert({x+i, y-i});
        if (obstruct != OPEN)
            break;
    }

    //Bottom left
    for (unsigned i=1; i<=spread; i++){
        obstruct = detect_obstruction(coord, {x-i, y+i}, board);
        if (obstruct == OPEN || obstruct == OPPONENT)
            poss_moves.insert({x-i, y+i});
        if (obstruct != OPEN)
            break;
    }

    //Bottom right
    for (unsigned i=1; i<=spread; i++){
        obstruct = detect_obstruction(coord, {x+i, y+i}, board);
        if (obstruct == OPEN || obstruct == OPPONENT)
            poss_moves.insert({x+i, y+i});
        if (obstruct != OPEN)
            break;
    }

    return poss_moves;
}

MOVESET Movesets::get_orthogonal_moves(COORD coord, Board * board, unsigned spread){
    unsigned x = coord[0];
    unsigned y = coord[1];
    MOVESET poss_moves;

    Movesets::OBSTRUCT_TYPE obstruct;

    //Up
    for (unsigned i=1; i<=spread; i++){
        obstruct = detect_obstruction(coord, {x, y+i}, board);
        if (obstruct == OPEN || obstruct == OPPONENT)
            poss_moves.insert({x, y+i});
        if (obstruct != OPEN)
            break;
    }

    //Down
    for (unsigned i=1; i<=spread; i++){
        obstruct = detect_obstruction(coord, {x, y-i}, board);
        if (obstruct == OPEN || obstruct == OPPONENT)
            poss_moves.insert({x, y-i});
        if (obstruct != OPEN)
            break;
    }

    //Left
    for (unsigned i=1; i<=spread; i++){
        obstruct = detect_obstruction(coord, {x-i, y}, board);
        if (obstruct == OPEN || obstruct == OPPONENT)
            poss_moves.insert({x-i, y});
        if (obstruct != OPEN)
            break;
    }

    //Right
    for (unsigned i=1; i<=spread; i++){
        obstruct = detect_obstruction(coord, {x+i, y}, board);
        if (obstruct == OPEN || obstruct == OPPONENT)
            poss_moves.insert({x+i, y});
        if (obstruct != OPEN)
            break;
    }

    return poss_moves;
}
