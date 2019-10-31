#pragma once

#include <ncursesw/curses.h>
#include "Tetromino.hpp"
#include "TetrominoType.hpp"
#include "size.hpp"

class World {
public:
    explicit World(WINDOW *win);

    void render();
    void set_offsets(int new_xoffset, int new_yoffset);
    void draw_tetromino(Tetromino const &tetromino);
    void clear_tetromino(Tetromino const &tetromino);

    [[nodiscard]] bool can_move_down(Tetromino &tetromino) const;
    [[nodiscard]] bool can_move_side(Tetromino &tetromino, int side) const;

private:
    enum Pair : short {
        P_NONE,
        P_SQUARE,
        P_L,
        P_RL
    };

    WINDOW *win;
    TetrominoType fields[FIELD_HEIGHT][FIELD_WIDTH] = { {TetrominoType::NONE} };
    Pair pairs[TetrominoType::LEN]{};
    int xoffset;
    int yoffset;

    void render_pixel(int y, int x, Pair pair);
    void draw_border();
    void set_field(int y, int x, TetrominoType type);
    [[nodiscard]] bool is_set(int y, int x)const ;
    void draw_tetromino_type(Tetromino const &tetromino, TetrominoType type);
};
