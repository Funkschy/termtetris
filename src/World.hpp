#pragma once

#include <ncursesw/curses.h>
#include "Tetromino.hpp"
#include "TetrominoType.hpp"
#include "size.hpp"

class World {
public:
    World(WINDOW *win);

    void render();
    void set_offsets(int new_xoffset, int new_yoffset);
    void draw_tetromino(Tetromino const &tetromino);
    void clear_tetromino(Tetromino const &tetromino);

private:
    WINDOW *win;
    TetrominoType fields[FIELD_HEIGHT][FIELD_WIDTH] = { {TetrominoType::NONE} };
    int xoffset;
    int yoffset;

    void render_pixel(int y, int x, unsigned short pair);
    void draw_border();
    void set_field(int y, int x, TetrominoType type);
    void draw_tetromino_type(Tetromino const &tetromino, TetrominoType type);
};
