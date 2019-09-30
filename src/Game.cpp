#include "Game.hpp"

const short SQUARE_PAIR = 1;

Game::Game(WINDOW *win): win(win), xoffset(0), yoffset(0) {
    init_pair(SQUARE_PAIR, COLOR_YELLOW, COLOR_YELLOW);
}

void Game::render_pixel(int y, int x, short pair) {
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


void Game::render() {
    for (int y = 0; y < FIELD_HEIGHT; y++) {
        for (int x = 0; x < FIELD_WIDTH; x++) {
            if (fields[y][x] == Game::SQUARE) {
                render_pixel(y, x, SQUARE_PAIR);
            }
        }
    }

    draw_border();
}

void Game::set_field(int y, int x, Game::FieldState state) {
    if (y >= 0 && x >= 0 && y < FIELD_HEIGHT && x < FIELD_WIDTH) {
        fields[y][x] = state;
    }
}

void Game::draw_border() {
    mvwvline(win, yoffset, xoffset, ACS_VLINE, FIELD_HEIGHT_PX + 1);
    mvwvline(win, yoffset, xoffset + FIELD_WIDTH_PX + 1, ACS_VLINE, FIELD_HEIGHT_PX + 1);

    mvwhline(win, yoffset, xoffset, ACS_HLINE, FIELD_WIDTH_PX + 1);
    mvwhline(win, yoffset + FIELD_HEIGHT_PX + 1, xoffset, ACS_HLINE, FIELD_WIDTH_PX + 1);

    mvwaddch(win, yoffset, xoffset, ACS_ULCORNER);
    mvwaddch(win, yoffset, xoffset + FIELD_WIDTH_PX + 1, ACS_URCORNER);

    mvwaddch(win, yoffset + FIELD_HEIGHT_PX + 1, xoffset, ACS_LLCORNER);
    mvwaddch(win, yoffset + FIELD_HEIGHT_PX + 1, xoffset + FIELD_WIDTH_PX + 1, ACS_LRCORNER);
}


void Game::set_offsets(int xoffset, int yoffset) {
    this->xoffset = xoffset;
    this->yoffset = yoffset;
}
