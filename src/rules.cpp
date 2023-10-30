#include "rules.h"

using namespace std;

bool Rules::is_in_check(COLOR color, Board * board) {
    COORD king_coord = get_king_coord(color, board);
    for (auto & [coord, piece] : board->items()){
        if (piece->color != NONE && piece->color != color){
            if (Movesets::get_moves(coord, board).contains(king_coord))
                return true;
        }
    }
    return false;
}

bool Rules::can_move(COLOR color, Board * board) {
    if (get_playable_piece_coords(color, board).empty())
        return false;
    return true;
}

COORDSET Rules::get_playable_piece_coords(COLOR color, Board * board) {
    COORDSET playable_pieces;
    for (auto & [coord, piece] : board->items()){
        if (piece->color == color){
            if (!get_moves(coord, board).empty())
                playable_pieces.insert(coord);
        }
    }
    return playable_pieces;
}

MOVELIST Rules::get_all_playable_moves(COLOR color, Board * board) {
    MOVELIST playable_moves;
    for (COORD source : get_playable_piece_coords(color, board)){
        COORDSET moves = get_moves(source, board);
        for (COORD target : moves){
            playable_moves.push_back({source, target});
        }
    }
    return playable_moves;
}

Rules::GAME_STATE Rules::get_game_state(COLOR color, Board * board) {
    // if only 2 kings left on the board, stalemate
    if (board->size() == NUM_PLAYERS)
        return STALEMATE;

    // Check if the king is in check
    bool in_check = is_in_check(color, board);
    // Check if any piece can move
    bool cn_move = can_move(color, board);

    // if in check and cannot move, checkmate
    if (in_check && !cn_move)
        return CHECKMATE;
    // if in check and can move, check
    else if (in_check && cn_move)
        return CHECK;
    // if cant move and not in check, stalemate
    else if (!in_check && !cn_move)
        return STALEMATE;
    // if not in check and can move, normal
    else
        return NORMAL;
}

COORDSET Rules::get_moves(COORD source, Board * board) {
    COORDSET poss_moves = Movesets::get_moves(source, board);
    poss_moves = chk_limit_moves(source, poss_moves, board);
    return poss_moves;
}

COORDSET Rules::chk_limit_moves(COORD source, COORDSET poss_moves, Board * board) {
    COORDSET new_poss_moves = poss_moves;
    Board * probe_board = board->copy();
    for (COORD target : poss_moves){
        // If the move is a castle and the king is in check, don't do it
        if (Movesets::is_castle(source, target, probe_board) && is_in_check(probe_board->get(source)->color, probe_board)){
            new_poss_moves.erase(target);
            continue;
        }
        
        // Make the move on the probe board
        Move * move = Move::make_move(source, target, probe_board);

        // Check if the move puts you in check
        if (is_in_check(probe_board->get(target)->color, probe_board)){
            new_poss_moves.erase(target);
        }

        // Undo the move
        move->undo_move(probe_board);

        delete move;
    }
    delete probe_board;

    return new_poss_moves;
}

COORD Rules::get_king_coord(COLOR color, Board * board) {
    for (auto & [coord, piece] : board->items()){
        if (piece->color == color && piece->type == KING)
            return coord;
    }
    throw runtime_error("King not found");
}
