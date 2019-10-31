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

void handle_input(bool &running, World &world, Tetromino &t) {
    int key = getch();
    switch (key) {
        case 'q':
            running = false;
            break;
        case 'r':
            t.rotate();
            break;
        case 'a':
            if (world.can_move_side(t, -1)) {
                t.set_x(t.x() - 1);

            }
            break;
        case 'd':
            if (world.can_move_side(t, 1)) {
                t.set_x(t.x() + 1);
            }
            break;
        default:
            break;
    }
}

int main() {
    WINDOW *win = init();
    if (!has_colors()) {
        std::cerr << "Unfortunately you terminal does not support colors" << std::endl;
        endwin();
        return -1;
    }

    World world(win);

    int x = FIELD_WIDTH / 2 - TETROMINO_WIDTH / 2, y = 0;
    Tetromino t(x, y, random_ty());

    bool running = true;
    int w, h;

    using namespace std::chrono_literals;
    auto drop_time = 500ms;
    auto start = std::chrono::system_clock::now();

    wclear(win);
    while (running) {
        curs_set(0);
        getmaxyx(win, h, w);

        auto now = std::chrono::system_clock::now();
        auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
        if (delta > drop_time) {
            start = now;
            y++;
        }

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

        world.draw_tetromino(t);
        world.set_offsets(xoffset, yoffset);
        world.render();
        world.clear_tetromino(t);

        handle_input(running, world, t);

        if (world.can_move_down(t)) {
            t.set_y(y);
        } else {
            x = FIELD_WIDTH / 2 - TETROMINO_WIDTH / 2;
            y = 0;
            world.draw_tetromino(t);
            t = Tetromino(x, y, random_ty());

            if (!world.can_move_down(t)) {
                goto EXIT;
            }
        }

        wrefresh(win);
    }

    EXIT:
    endwin();
    return 0;
}
