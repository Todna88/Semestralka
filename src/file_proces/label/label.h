/**
 * \file label.h
 * \author Antonín Hobl (hobl@students.zcu.cz)
 * \brief Hlavičkový soubor s deklaracemi funkcí pro práci se sekcemi vstupního souboru.
 * \version 1.0
 * \date 2025-01-03
 */

#ifndef LABEL_H
#define LABEL_H

#include"../../errors.h"

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define LINE_LENGTH 1024

#define GENERALS_SPLITTER " "

#define END_CHAR_LEN 1
#define ENDING_CHAR '\0'
#define NEWLINE "\n"
#define NEWLINE_WINDOWS "\r\n"
#define COMMENT "\\"
#define NEWLINE_CHAR '\n'

#define NEWLINE_LEN 1
#define NEWLINE_WINDOWS_LEN 2


/** \brief Makro výmaz komtentářů z řádky. */
#define DELETE_COMMS(line)          \
    switch (delete_comments(line)){ \
        case 0:                     \
            return NULL;            \
            break;                  \
        case 2:                     \
            continue;               \
            break;                  \
        }                           

/** \brief Makro pro kontrolu délky řádky. */
#define CHECK_LINE_LENGTH(line, file)                                               \
    if(!strchr(line,'\n') && !feof(file) && !strpbrk(line, "\r\n")){                \
            error = LINE_ERR;                                                       \
            return NULL;                                                            \
        }

/** \brief Definice datového typu znaku reprezentující typ sekce. */
typedef char label_type;

/** \brief Definice struktury uchovávající potředná data o sekci */
struct label{
    char *name;
    label_type label;
};

/**
 * \brief Funkce zjistí typ sekce z předaného řetězce `line`.
 * \param line Řetězec obsahující název některé se sekcí jako podřetězec.
 * \return label_type typ sekce, nebo 0 při chybě.
 */
label_type identify_label(const char *line);

/**
 * \brief Funkce vymaže komentaře z předaného řetězce `line`.
 * \param line Řetězec, ve kterém se vymažou komentáře.
 * \return int 2, pokud je komentář na začátku řádky, 1, pokud se komentář vymazal, 0 při chybě.
 */
int delete_comments(char *line);

#endif