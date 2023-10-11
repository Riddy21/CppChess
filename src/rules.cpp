#include "rules.h"

using namespace std;

bool Rules::is_in_check(COLOR color, Board * board) {
    COORD king_coord = get_king_coord(color, board);
}

Rules::GAME_STATE Rules::get_game_state(COLOR color, Board * board) {
}

MOVESET Rules::get_moves(COORD source, Board * board) {
}

MOVESET Rules::chk_limit_moves(COORD source, MOVESET poss_moves, Board * board) {
}

COORD Rules::get_king_coord(COLOR color, Board * board) {
    for (auto & [key, value] : board->items()){
        printf("%d, %d\n", key[0], key[1]);
        printf("%c\n", value->type);
    }
    return {0, 0};
}
