#include "Tetromino.hpp"

#include <cstring>
#include "size.hpp"

Tetromino::Tetromino(int x, int y, TetrominoType type)
        : m_x(x), m_y(y), m_type(type) {

    memset(m_content, false, TETROMINO_HEIGHT * TETROMINO_WIDTH * sizeof(bool));

    switch (m_type) {
        case TetrominoType::SQUARE:
            m_content[1][1] = true;
            m_content[1][2] = true;
            m_content[2][1] = true;
            m_content[2][2] = true;
            break;
        case TetrominoType::L:
            m_content[0][1] = true;
            m_content[1][1] = true;
            m_content[2][1] = true;
            m_content[2][2] = true;
            break;
        case TetrominoType::RL:
            m_content[0][1] = true;
            m_content[1][1] = true;
            m_content[2][1] = true;
            m_content[2][0] = true;
            break;
        case TetrominoType::I:
            m_content[0][1] = true;
            m_content[1][1] = true;
            m_content[2][1] = true;
            m_content[3][1] = true;
            break;
        case TetrominoType::Z:
            m_content[1][1] = true;
            m_content[1][2] = true;
            m_content[2][2] = true;
            m_content[2][3] = true;
            break;
        case TetrominoType::PYRAMID:
            m_content[2][2] = true;
            m_content[3][1] = true;
            m_content[3][2] = true;
            m_content[3][3] = true;
            break;
        case NONE:
        case LEN:
            break;
    }
}

void Tetromino::clear_largest_coords() {
    for (auto &m_largest_coord : m_largest_coords) {
        m_largest_coord = std::pair(-1, -1);
    }
}

int Tetromino::find_largest_x(bool const (&arr)[TETROMINO_HEIGHT][TETROMINO_WIDTH]) {
    for (int x = TETROMINO_WIDTH - 1; x >= 0; x--) {
        for (auto &y : arr) {
            if (y[x]) {
                return x;
            }
        }
    }

    return -1;
}

int Tetromino::find_smallest_x(bool const (&arr)[TETROMINO_HEIGHT][TETROMINO_WIDTH]) {
    for (int x = 0; x < TETROMINO_WIDTH; x++) {
        for (auto &y : arr) {
            if (y[x]) {
                return x;
            }
        }
    }

    return -1;
}

void Tetromino::set_x(int x) {
    if (
        // right
            (x > m_x && x < (FIELD_WIDTH - find_largest_x(m_content)))
            // left
            || (x < m_x && (m_x + find_smallest_x(m_content)) > 0)
            ) {
        m_x = x;
    }
}

void Tetromino::set_y(int y) {
    m_y = y;
}

void Tetromino::find_largest_y_coords() {
    clear_largest_coords();

    for (int y = TETROMINO_HEIGHT - 1; y >= 0; y--) {
        for (int x = 0; x < TETROMINO_WIDTH; x++) {
            if (m_content[y][x] && m_largest_coords[x].first < 0) {
                m_largest_coords[x] = std::pair(x, y);
            }
        }
    }
}

void Tetromino::find_largest_x_coords() {
    clear_largest_coords();

    for (int y = 0; y < TETROMINO_HEIGHT; y++) {
        for (int x = TETROMINO_WIDTH - 1; x >= 0; x--) {
            if (m_content[y][x] && m_largest_coords[y].first < 0) {
                m_largest_coords[y] = std::pair(x, y);
            }
        }
    }
}

void Tetromino::find_smallest_x_coords() {
    clear_largest_coords();

    for (int y = 0; y < TETROMINO_HEIGHT; y++) {
        for (int x = 0; x < TETROMINO_WIDTH; x++) {
            if (m_content[y][x] && m_largest_coords[y].first < 0) {
                m_largest_coords[y] = std::pair(x, y);
            }
        }
    }
}

std::pair<int, int> const *Tetromino::largest_coords() const {
    return m_largest_coords;
}

void Tetromino::gen_rotate(bool (&arr)[4][4]) {
    for (int y = 0; y < TETROMINO_HEIGHT; y++) {
        for (int x = 0; x < TETROMINO_WIDTH; x++) {
            arr[TETROMINO_HEIGHT - x - 1][y] = m_content[y][x];
        }
    }
}

void Tetromino::set_content(bool (&new_content)[TETROMINO_HEIGHT][TETROMINO_WIDTH]) {
    memcpy(m_content, new_content, sizeof(new_content));
}

