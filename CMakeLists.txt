cmake_minimum_required(VERSION 3.5)

project(Semestralka LANGUAGES C)
set(CMAKE_C_FLAGS "-Wall -Wextra -pedantic -ansi")

add_executable(lp
    src/lp.c
    src/file_proces/file_proces.c
    src/flags/flags.c
    src/file_proces/label/label.c
    src/file_proces/label/generals/generals.c
    #src/stack/stack_check.c
    # Podadresář s implementací zásobníku.
    #src/stack/stack.c
    #src/node/node.c
)