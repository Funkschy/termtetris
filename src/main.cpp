#include <ncursesw/curses.h>

#include <iostream>
#include <chrono>
#include <clocale>

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

void handle_input(bool &running, World &world, Tetromino &t, bool &double_speed) {
    double_speed = false;

    int key = getch();
    switch (key) {
        case 'q':
            running = false;
            break;
        case 'r':
            t.rotate();
            break;
        case 's':
            double_speed = true;
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

void check_dims(WINDOW *win, int w, int h, bool &dim_check) {
    if (w < FIELD_WIDTH_PX || h < FIELD_HEIGHT_PX) {
        wclear(win);
        wprintw(
                win,
                "Please resize the window to %dx%d\n",
                FIELD_WIDTH_PX,
                FIELD_HEIGHT_PX
        );

        wrefresh(win);
        dim_check = false;
        return;
    }

    if (!dim_check) {
        wclear(win);
    }

    dim_check = true;
}

void write_score(WINDOW *score_window, unsigned int score) {
    wclear(score_window);
    wprintw(
            score_window,
            "Score: %d\n",
            score
    );
    wrefresh(score_window);
}

int main() {
    WINDOW *game_window = init();
    WINDOW *score_window = newwin(10, 10, 0, 0);

    if (!has_colors()) {
        std::cerr << "Unfortunately you terminal does not support colors" << std::endl;
        endwin();
        return -1;
    }

    World world(game_window);

    int x = FIELD_WIDTH / 2 - TETROMINO_WIDTH / 2, y = 0;
    Tetromino t(x, y, random_ty());

    bool running = true;

    int w, h, old_w, old_h;
    getmaxyx(game_window, h, w);
    old_w = w;
    old_h = h;

    using namespace std::chrono_literals;
    auto drop_time = 500ms;
    auto start = std::chrono::system_clock::now();

    unsigned int score = 0;

    bool dim_check = true;
    bool touched_ground = false;
    bool passed = false;
    bool double_speed = false;

    wclear(game_window);
    while (running) {
        curs_set(0);

        getmaxyx(game_window, h, w);
        if (old_w != w || old_h != h) {
            wclear(game_window);
        }

        old_w = w;
        old_h = h;

        check_dims(game_window, w, h, dim_check);
        if (!dim_check) {
            continue;
        }

        int xoffset = (w - FIELD_WIDTH_PX) / 2;
        int yoffset = (h - FIELD_HEIGHT_PX) / 2;

        world.draw_tetromino(t);
        world.set_offsets(xoffset, yoffset);
        world.render();
        world.clear_tetromino(t);

        handle_input(running, world, t, double_speed);

        auto now = std::chrono::system_clock::now();
        auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);

        auto drop_div = double_speed ? 2 : 1;

        if (delta > drop_time / drop_div) {
            start = now;
            y++;

            if (touched_ground) {
                passed = true;
            }
        }

        if (world.can_move_down(t)) {
            t.set_y(y);
        } else {
            // give player another chance to move by waiting for another drop_time
            // to pass
            if (!touched_ground) {
                touched_ground = true;
            }

            if (!passed) {
                continue;
            }
            passed = false;
            touched_ground = false;

            world.draw_tetromino(t);
            score += world.delete_lines();
            write_score(score_window, score);

            x = FIELD_WIDTH / 2 - TETROMINO_WIDTH / 2;
            y = 0;

            t = Tetromino(x, y, random_ty());
            if (!world.can_move_down(t)) {
                goto EXIT;
            }
        }

        wrefresh(game_window);
        wrefresh(score_window);
    }

    EXIT:
    delwin(score_window);
    endwin();
    return 0;
}
