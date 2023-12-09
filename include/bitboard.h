#ifdef SWIG

%module bitboard
%{
#include "bitboard.h"
%}

%include "settings.h";

#endif

#ifndef BITBOARD_H
#define BITBOARD_H
#pragma once // Make sure it's compiled only once

#include "settings.h"

using namespace std;

#endif