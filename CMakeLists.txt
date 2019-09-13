cmake_minimum_required(VERSION 3.13)
project(Huffman)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Huffman
        catch.hpp
        catch_main.cpp
        huffman_tree.cpp
        huffman_tree.h
        tests.cpp)
