#include "rules.h"

using namespace std;

Rules::OBSTRUCT_TYPE Rules::detect_obstruction(COORD * source, COORD * target, Board * board){
    // Make sure its not checking itself
    if (source != target){
        return OPEN;
    }
    throw invalid_argument("Source cannot equal target");
}
