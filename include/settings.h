#ifdef SWIG

%module settings
%{
#include "settings.h"
%}

%include <std_array.i>;
%include <std_except.i>;


%template(COORD) std::array<int, 2>;

// Catch all exceptions in C++ and put them as Runtime errors
%exception {
    try {
        $action
    } catch (const std::exception &e) {
        SWIG_exception(SWIG_RuntimeError, e.what());
    }
}

#endif
#ifndef SETTINGS_H
#define SETTINGS_H

#include <array>

#define DEFAULT_BOARD_PRESETS_PATH "Presets/default.txt"
#define BOARD_HEIGHT 8
#define BOARD_WIDTH 8
typedef std::array<int, 2> COORD;
enum COLORS{
    WHITE,
    BLACK,
};

#endif
