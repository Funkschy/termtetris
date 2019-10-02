#pragma once

#include "TetrominoType.hpp"

const int TETROMINO_WIDTH = 4;
const int TETROMINO_HEIGHT = 4;

class Tetromino {
public:
    Tetromino(int x, int y, TetrominoType type);

    void set_x(int x);
    int const &x() const { return mx; };

    void set_y(int y);
    int const &y() const { return my; };

    TetrominoType &type() { return mtype; }
    TetrominoType const &type() const { return mtype; }

    bool is_set(int y, int x) const { return content[y][x]; }
private:
    int mx;
    int my;
    TetrominoType mtype;
    bool content[TETROMINO_HEIGHT][TETROMINO_WIDTH] = { false };

    int find_smallest_x();
    int find_largest_x();
};
