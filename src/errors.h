/**
 * \file errors.h
 * \author Antonín Hobl (hobl@students.zcu.cz)
 * \brief Hlavičkový soubor s chybovými hlášeními.
 * \version 1.0
 * \date 2024-10-31
 */

#ifndef ERRORS_H
#define ERRORS_H

/** \brief Makra pro jednotlivá chybová hlášení. */
#define INPUT_FILE_ERR 1
#define OUTPUT_FILE_ERR 2
#define VARIABLE_ERR 10
#define SYNTAX_ERR 11
#define FUNC_BOUND_ERR 20
#define SOLUTION_ERR 21
#define MEMORY_ERR 3
#define LINE_ERR 4
#define VAR_NAME_ERR 5
#define POINTER_ERR 7
#define ARGUMENT_ERR 6
#define FLAG_ERR 8

/** \brief Definice datového typu celé kladné číslo pro uložení kódu chyby. */
typedef unsigned int error_type;

/** \brief Definice globální proměnné pro uložení kódu chyby */
extern error_type error;

#endif