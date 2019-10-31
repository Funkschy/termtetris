#include "TetrominoType.hpp"

#include <random>

TetrominoType random_ty() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> dist(1, LEN - 1);

    return (TetrominoType) dist(rng);
}
