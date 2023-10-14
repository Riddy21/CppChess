#ifdef SWIG

%module game
%{
#include "game.h"
%}
%include "settings.h"
%include "movesets.h"
%include "move.h"
%include "board.h"
%include "rules.h"
%include "piece.h"

#endif
#ifndef GAME_H
#define GAME_H
#pragma once // Make sure it's compiled only once

#include "settings.h"
#include "movesets.h"
#include "move.h"
#include "board.h"
#include "piece.h"
#include "rules.h"

using namespace std;

class Game {
public:

private:

};
#endif