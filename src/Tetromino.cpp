#include "Tetromino.hpp"

#include "size.hpp"

Tetromino::Tetromino(int x, int y, TetrominoType type): mx(x), my(y), mtype(type) {
    switch(mtype) {
        case TetrominoType::SQUARE:
            content[1][1] = true;
            content[1][2] = true;
            content[2][1] = true;
            content[2][2] = true;
            break;
        default: break;
    }
}

int Tetromino::find_largest_x() {
    for (int x = TETROMINO_WIDTH - 1; x >= 0; x--) {
        for (int y = 0; y < TETROMINO_HEIGHT; y++) {
            if (is_set(y, x)) {
                return x;
            }
        }
    }

    return -1;
}

int Tetromino::find_smallest_x() {
    for (int x = 0; x < TETROMINO_WIDTH; x++) {
        for (int y = 0; y < TETROMINO_HEIGHT; y++) {
            if (is_set(y, x)) {
                return x;
            }
        }
    }

    return -1;
}

void Tetromino::set_x(int x) {
    if (
            // right
            (x > mx && x < (FIELD_WIDTH - find_largest_x()))
            // left
            || (x < mx && (mx + find_largest_x()) >= 2)
    ) {
        mx = x;
    }
}

void Tetromino::set_y(int y) {
    my = y;
}
