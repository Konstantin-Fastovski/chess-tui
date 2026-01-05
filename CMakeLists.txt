cmake_minimum_required(VERSION 4.2)
project(chess_tui)

set(CMAKE_CXX_STANDARD 23)

add_executable(chess_tui
        board.hpp
        main.cpp
)
