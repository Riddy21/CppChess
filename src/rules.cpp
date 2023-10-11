#include "rules.h"

using namespace std;

bool Rules::is_in_check(COLOR color, Board * board) {
    COORD king_coord = get_king_coord(color, board);
    for (auto & [coord, piece] : board->items()){
        if (piece->color != color){
            if (Movesets::get_moves(coord, board).contains(king_coord))
                return true;
        }
    }
    return false;
}

Rules::GAME_STATE Rules::get_game_state(COLOR color, Board * board) {
}

MOVESET Rules::get_moves(COORD source, Board * board) {
}

MOVESET Rules::chk_limit_moves(COORD source, MOVESET poss_moves, Board * board) {
    // FIXME: Can you do this without the move object?
    //   ans: Yes, you can, add a move object
    //        dependds on the moveset class, should not contain check limiting
    //        Have check limiting be managed at game level
}

COORD Rules::get_king_coord(COLOR color, Board * board) {
    for (auto & [coord, piece] : board->items()){
        if (piece->color == color && piece->type == KING)
            return coord;
    }
    throw runtime_error("King not found");
}
