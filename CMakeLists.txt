cmake_minimum_required(VERSION 3.16)
project(mapsrunner C)

set(CMAKE_C_STANDARD 99)

add_executable(mapsrunner main.c dijkstra.c dijkstra.h loader.c loader.h)