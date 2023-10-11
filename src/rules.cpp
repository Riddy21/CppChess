#include "rules.h"

using namespace std;

bool Rules::is_in_check(COLOR color, Board * board) {
    COORD king_coord = get_king_coord(color, board);
    return false;
}

Rules::GAME_STATE Rules::get_game_state(COLOR color, Board * board) {
}

MOVESET Rules::get_moves(COORD source, Board * board) {
}

MOVESET Rules::chk_limit_moves(COORD source, MOVESET poss_moves, Board * board) {
}

COORD Rules::get_king_coord(COLOR color, Board * board) {
    for (auto & [key, value] : board->items()){
        if (value->color == color && value->type == KING)
            return key;
    }
    throw runtime_error("King not found");
}
