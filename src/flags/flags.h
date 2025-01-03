/**
 * \file flags.h
 * \author Antonín Hobl (hobl@students.zcu.cz)
 * \brief Hlavičkový soubor s deklaracemi funkcí pro práci s přepínači z argumentu souboru.
 * \version 1.0
 * \date 2025-01-02
 */

#ifndef FLAGS_H
#define FLAGS_H

#include <stdio.h>

/** \brief Definice datového typu struktura obsahující přesnou syntaxi přepínačů. */
typedef struct flags{
    char *o;
    char *output;
} flags_type;

/**
 * \brief Funkce rozhodne, zda je předaný řetězec `flag` přepínač.
 * \param flag Řetězec kontrolovaný na přepínač.
 * \return int 1 pokud se jedná o přepínač, jinak 0.
 */
int is_flag(const char *flag);

#endif