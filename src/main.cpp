#define _XOPEN_SOURCE_EXTENDED
#include <ncursesw/curses.h>

#include <iostream>
#include <clocale>
#include <cstdlib>

#include "Game.hpp"

WINDOW *init() {
    // make this work with all different locales
    setlocale(LC_ALL, "");

    WINDOW *w = initscr();
    // disable line buffering
    cbreak();
    // dont echo typed chars
    noecho();
    start_color();

    // enable curses to detect return key
    nonl();
    intrflush(w, false);
    keypad(w, true);
    return w;
}

int main(void) {
    WINDOW *win = init();
    if (!has_colors()) {
        std::cerr << "Unfortunately you terminal does not support colors" << std::endl;
        endwin();
        return -1;
    }

    Game game(win);

    int x = 0, y = 0;

    int w, h;
    while(true) {
        wclear(win);
        curs_set(0);
        getmaxyx(win, h, w);

        if (w < FIELD_WIDTH_PX || h < FIELD_HEIGHT_PX) {
            wprintw(
                win,
                "Please resize the window to %dx%d\n",
                FIELD_WIDTH_PX,
                FIELD_HEIGHT_PX
            );
            wrefresh(win);
            continue;
        }

        int xoffset = (w - FIELD_WIDTH_PX) / 2;
        int yoffset = (h - FIELD_HEIGHT_PX) / 2;

        game.set_field(y, x, Game::EMPTY);
        game.set_field(y, x, Game::SQUARE);
        game.set_offsets(xoffset, yoffset);
        game.render();

        int key = getch();
        if (key == 'q') {
            break;
        }

        wrefresh(win);
    }

    endwin();

    return 0;
}
