#include "rules.h"

using namespace std;

Rules::OBSTRUCT_TYPE Rules::detect_obstruction(COORD source, COORD target, Board * board){
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
