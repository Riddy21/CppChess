#ifdef SWIG

%module rules
%{
#include "rules.h"
%}
%include "board.h"

#endif
#ifndef RULES_H
#define RULES_H

#include <stdio.h>
#include <stdexcept>

#include "board.h"

//namespace logging = spdlog;

namespace Rules {
    enum OBSTRUCT_TYPE{
        SELF,
        OPPONENT,
        OPEN,
    };

    /**
     * @brief Detect if the piece is obstructed
     * 
     * @return OBSTRUCT_TYPE
     */
    OBSTRUCT_TYPE detect_obstruction(COORD * source, COORD * target, Board * board);
};

#endif