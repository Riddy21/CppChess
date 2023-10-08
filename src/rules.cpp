#include "rules.h"

using namespace std;

bool Rules::is_out_of_bounds(COORD coord){
    return (coord[0] < 0 || coord[0] >= BOARD_WIDTH || coord[1] < 0 || coord[1] >= BOARD_HEIGHT);
}

Rules::OBSTRUCT_TYPE Rules::detect_obstruction(COORD source, COORD target, Board * board){
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

MOVESET Rules::get_pawn_moves(COORD source, Board * board){
    unsigned x = source[0];
    unsigned y = source[1];
    MOVESET poss_moves;
    Rules::OBSTRUCT_TYPE obstruct;

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
    } else {
        throw invalid_argument("Trying to get pawn moves on blank piece");
    }

    // Adding enpassant moves

    return poss_moves;
}
