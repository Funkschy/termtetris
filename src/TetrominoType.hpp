#pragma once

#include <ncursesw/curses.h>

enum TetrominoType : int {
    NONE = 0,
    SQUARE,
    L,
    RL,

    LEN // not really a type, just a marker
};

TetrominoType random_ty();
