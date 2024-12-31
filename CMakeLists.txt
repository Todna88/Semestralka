cmake_minimum_required(VERSION 3.5)

project(Semestralka LANGUAGES C)
set(CMAKE_C_FLAGS "-Wall -Wextra -pedantic -ansi")

add_executable(lp
    src/lp.c
    src/file_proces/file_proces.c
    src/flags/flags.c
    src/file_proces/label/label.c
    src/file_proces/label/generals/generals.c
    src/stack/stack.c
    src/node/node.c
    src/file_proces/label/function/function.c
    src/file_proces/label/function/operators.c
    src/file_proces/label/subject_to/subject_to.c
    src/file_proces/label/bounds/bounds.c
    src/simplex/simplex.c
)