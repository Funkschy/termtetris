#include "World.hpp"

World::World(WINDOW *win) : win(win), xoffset(0), yoffset(0) {
    init_pair(Pair::P_NONE, COLOR_BLACK, COLOR_BLACK);
    init_pair(Pair::P_SQUARE, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(Pair::P_L, COLOR_GREEN, COLOR_GREEN);
    init_pair(Pair::P_RL, COLOR_BLUE, COLOR_BLUE);

    pairs[TetrominoType::NONE] = Pair::P_NONE;
    pairs[TetrominoType::SQUARE] = Pair::P_SQUARE;
    pairs[TetrominoType::L] = Pair::P_L;
    pairs[TetrominoType::RL] = Pair::P_RL;
}

void World::render_pixel(int y, int x, Pair pair) {
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
            auto type = fields[y][x];
            render_pixel(y, x, pairs[type]);
        }
    }

    draw_border();
}

void World::set_field(int y, int x, TetrominoType type) {
    if (y >= 0 && x >= 0 && y < FIELD_HEIGHT && x < FIELD_WIDTH) {
        fields[y][x] = type;
    }
}

bool World::is_set(int y, int x) const {
    return fields[y][x] != TetrominoType::NONE;
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

bool World::can_move_down(Tetromino &tetromino) const {
    tetromino.find_largest_y_coords();

    int x = tetromino.x();
    int y = tetromino.y();
    auto largest = tetromino.largest_coords();

    for (int i = 0; i < TETROMINO_WIDTH; i++) {
        auto[rel_x, rel_y] = largest[i];
        if (rel_x < 0) {
            continue;
        }

        int next_y = rel_y + y + 1;

        if (next_y >= FIELD_HEIGHT) {
            return false;
        }

        int real_x = rel_x + x;
        if (is_set(next_y, real_x)) {
            return false;
        }
    }

    return true;
}

bool World::can_move_side(Tetromino &tetromino, int side) const {
    if (side < 0) {
        tetromino.find_smallest_x_coords();
    } else {
        tetromino.find_largest_x_coords();
    }

    int x = tetromino.x();
    int y = tetromino.y();
    auto largest = tetromino.largest_coords();

    for (int i = 0; i < TETROMINO_WIDTH; i++) {
        auto[rel_x, rel_y] = largest[i];
        if (rel_x < 0) {
            continue;
        }

        int next_x = rel_x + x + side;
        int real_y = rel_y + y;

        if (is_set(real_y, next_x)) {
            return false;
        }
    }

    return true;
}
