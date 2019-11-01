#pragma once

#include <utility>

#include "TetrominoType.hpp"

const int TETROMINO_WIDTH = 4;
const int TETROMINO_HEIGHT = 4;

class Tetromino {
public:
    Tetromino(int x, int y, TetrominoType type);

    void set_x(int x);

    [[nodiscard]] int const &x() const { return m_x; };

    void set_y(int y);

    [[nodiscard]] int const &y() const { return m_y; };

    TetrominoType &type() { return m_type; }

    [[nodiscard]] TetrominoType const &type() const { return m_type; }

    [[nodiscard]] bool is_set(int y, int x) const { return m_content[y][x]; }

    [[nodiscard]] std::pair<int, int> const *largest_coords() const;

    void find_largest_y_coords();

    void find_smallest_x_coords();

    void find_largest_x_coords();

    void set_content(bool (&new_content)[TETROMINO_HEIGHT][TETROMINO_WIDTH]);

    void gen_rotate(bool (&arr)[TETROMINO_HEIGHT][TETROMINO_WIDTH]);

private:
    int m_x;
    int m_y;
    TetrominoType m_type;
    bool m_content[TETROMINO_HEIGHT][TETROMINO_WIDTH]{};
    std::pair<int, int> m_largest_coords[TETROMINO_WIDTH];

    void clear_largest_coords();

    static int find_smallest_x(bool const (&arr)[TETROMINO_HEIGHT][TETROMINO_WIDTH]);

    static int find_largest_x(bool const (&arr)[TETROMINO_HEIGHT][TETROMINO_WIDTH]);

};
