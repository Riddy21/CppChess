#ifdef SWIG
%module chesslib
%{
#include "settings.h"
#include "piece.h"
#include "board.h"
#include "movesets.h"
#include "move.h"
#include "rules.h"
%}

%include "settings.h"
%include "piece.h"
%include "board.h"
%include "movesets.h"
%include "move.h"
%include "rules.h"

#endif