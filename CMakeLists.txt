cmake_minimum_required(VERSION 4.2)
project(chess_tui)

set(CMAKE_CXX_STANDARD 23)

add_executable(chess_tui
        src/main.cpp
        include/chess-tui/board.hpp
        include/chess-tui/vector.hpp
        src/vector.cpp
        include/chess-tui/piece.hpp
        src/piece.cpp
        include/chess-tui/piece-visitor.hpp
        src/board.cpp
        src/piece-visitor.cpp
        include/chess-tui/player.hpp
        src/player.cpp
)
target_include_directories(chess_tui PUBLIC include)
