cmake_minimum_required(VERSION 4.2)
project(chess_tui)

set(CMAKE_CXX_STANDARD 23)

add_executable(chess_tui
        main.cpp
        board.hpp
        include/chess-tui/vector.hpp
        src/vector.cpp
        include/chess-tui/piece.hpp
        src/piece.cpp
)
target_include_directories(chess_tui PUBLIC include)
