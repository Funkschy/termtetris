#define _XOPEN_SOURCE_EXTENDED
#include <ncursesw/curses.h>

#include <iostream>
#include <chrono>
#include <thread>
#include <clocale>
#include <cstdlib>

#include "World.hpp"
#include "size.hpp"
#include "TetrominoType.hpp"

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
    nodelay(w, true);
    intrflush(w, false);
    keypad(w, true);
    return w;
}

int main() {
    WINDOW *win = init();
    if (!has_colors()) {
        std::cerr << "Unfortunately you terminal does not support colors" << std::endl;
        endwin();
        return -1;
    }

    World game(win);

    int x = -1, y = -2;
    Tetromino t(x, y, TetrominoType::SQUARE);

    bool running = true;
    int w, h;

    using namespace std::chrono_literals;
    while(running) {
        wclear(win);
        curs_set(0);
        getmaxyx(win, h, w);

        y++;

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

        game.draw_tetromino(t);
        game.set_offsets(xoffset, yoffset);
        game.render();
        game.clear_tetromino(t);

        int key = getch();
        switch(key) {
            case 'q': running = false; break;
            case 'a': t.set_x(t.x() - 1); break;
            case 'd': t.set_x(t.x() + 1); break;
            default: break;
        }

        t.set_y(y);

        wrefresh(win);
        // if we sleep less than that, we get screen flickering
        std::this_thread::sleep_for(47ms);
    }

    endwin();

    return 0;
}
