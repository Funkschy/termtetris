cmake_minimum_required(VERSION 3.5)
set(PROJ_NAME termtetris)
project(${PROJ_NAME})

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJ_NAME} src/main.cpp src/World.cpp src/Tetromino.cpp src/TetrominoType.cpp)
target_link_libraries(${PROJ_NAME} ncursesw)
target_compile_options(${PROJ_NAME} PRIVATE -Werror -Wall -Wextra -Wconversion -pedantic)
