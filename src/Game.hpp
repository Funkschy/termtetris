#pragma once

#include <ncursesw/curses.h>

const int PIXEL_SIZE_W = 4;
const int PIXEL_SIZE_H = 2;

const int FIELD_WIDTH = 10;
const int FIELD_HEIGHT = 15;

const int FIELD_WIDTH_PX = PIXEL_SIZE_W * FIELD_WIDTH;
const int FIELD_HEIGHT_PX = PIXEL_SIZE_H * FIELD_HEIGHT;

class Game {
public:
    enum FieldState {
        EMPTY = 0,
        SQUARE = 1
    };

    Game(WINDOW *win);

    void render();
    void set_field(int y, int x, Game::FieldState state);
    void set_offsets(int xoffset, int yoffset);

private:
    WINDOW *win;
    FieldState fields[FIELD_HEIGHT][FIELD_WIDTH] = { EMPTY };
    int xoffset;
    int yoffset;

    void render_pixel(int y, int x, short pair);
    void draw_border();
};
