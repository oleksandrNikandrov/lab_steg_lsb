cmake_minimum_required(VERSION 3.27)
project(steg_lsb C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

file(GLOB SOURCES "src/*.c")

add_executable(steg_lsb ${SOURCES}
        include/cli/alg_handlers.h)