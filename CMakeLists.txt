cmake_minimum_required(VERSION 3.5)

project(semestralka LANGUAGES C)
set(CMAKE_C_FLAGS "-Wall -Wextra -pedantic -ansi")

add_executable(



    # Podadresář s implementací zásobníku.
    src/stack/stack.c
)