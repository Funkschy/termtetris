#include "World.hpp"

const short SQUARE_PAIR = 1;

World::World(WINDOW *win): win(win), xoffset(0), yoffset(0) {
    init_pair(SQUARE_PAIR, COLOR_YELLOW, COLOR_YELLOW);
}

void World::render_pixel(int y, int x, unsigned short pair) {
    y = y * PIXEL_SIZE_H + 1 + yoffset;
    x = x * PIXEL_SIZE_W + 1 + xoffset;

    attron(COLOR_PAIR(pair));
    for (int h = 0; h < PIXEL_SIZE_H; h++) {
        for (int w = 0; w < PIXEL_SIZE_W; w++) {
            mvwadd_wch(win, y + h, x + w, WACS_BLOCK);
        }
    }
    attroff(COLOR_PAIR(pair));
}


void World::render() {
    for (int y = 0; y < FIELD_HEIGHT; y++) {
        for (int x = 0; x < FIELD_WIDTH; x++) {
            if (fields[y][x] == TetrominoType::SQUARE) {
                render_pixel(y, x, SQUARE_PAIR);
            }
        }
    }

    draw_border();
}

void World::set_field(int y, int x, TetrominoType type) {
    if (y >= 0 && x >= 0 && y < FIELD_HEIGHT && x < FIELD_WIDTH) {
        fields[y][x] = type;
    }
}

void World::draw_border() {
    mvwvline(win, yoffset, xoffset, ACS_VLINE, FIELD_HEIGHT_PX + 1);
    mvwvline(win, yoffset, xoffset + FIELD_WIDTH_PX + 1, ACS_VLINE, FIELD_HEIGHT_PX + 1);

    mvwhline(win, yoffset, xoffset, ACS_HLINE, FIELD_WIDTH_PX + 1);
    mvwhline(win, yoffset + FIELD_HEIGHT_PX + 1, xoffset, ACS_HLINE, FIELD_WIDTH_PX + 1);

    mvwaddch(win, yoffset, xoffset, ACS_ULCORNER);
    mvwaddch(win, yoffset, xoffset + FIELD_WIDTH_PX + 1, ACS_URCORNER);

    mvwaddch(win, yoffset + FIELD_HEIGHT_PX + 1, xoffset, ACS_LLCORNER);
    mvwaddch(win, yoffset + FIELD_HEIGHT_PX + 1, xoffset + FIELD_WIDTH_PX + 1, ACS_LRCORNER);
}


void World::set_offsets(int new_xoffset, int new_yoffset) {
    this->xoffset = new_xoffset;
    this->yoffset = new_yoffset;
}

void World::draw_tetromino_type(Tetromino const &tetromino, TetrominoType type) {
    for (int y = tetromino.y(); y < tetromino.y() + TETROMINO_HEIGHT; y++) {
        for (int x = tetromino.x(); x < tetromino.x() + TETROMINO_WIDTH; x++) {
            if (tetromino.is_set(y - tetromino.y(), x - tetromino.x())) {
                set_field(y, x, type);
            }
        }
    }
}

void World::draw_tetromino(Tetromino const &tetromino) {
    draw_tetromino_type(tetromino, tetromino.type());
}

void World::clear_tetromino(Tetromino const &tetromino) {
    draw_tetromino_type(tetromino, TetrominoType::NONE);
}
