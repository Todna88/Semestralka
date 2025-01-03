/**
 * \file operators.h
 * \author Antonín Hobl (hobl@students.zcu.cz)
 * \brief Hlavičkový soubor s deklaracemi funkcí nutných pro práci s danými operátory.
 * \version 1.0
 * \date 2025-01-02
 */

#ifndef OPERATORS_H
#define OPERATORS_H

#include "../label.h"

#define PLUS '+'
#define MINUS '-'
#define MULTIPLY '*'
#define LOWER '<'
#define HIGHER '>'
#define EQUAL '='

/** \brief Definice datového typu ukazatele na funkci vykonávající aritmetickou operaci. */
typedef double (*operation)(double, double);

/** \brief Definice struktury, která uchovává všechny potřebná data o daném aritmetickém operátoru */
struct operators{
    char operator;      /*!< Znak, kterým je operace popsána. */
    size_t priority;    /*!< Priorita operátoru. */
    operation operate;  /*!< Přidružená aritmetická operace. */
};

/**
 * \brief Funkce zjistní prioritu daného aritmetického operátoru.
 * \param operator Znak, který obsahuje daný aritmetický operátor.
 * \return int priorita operátoru, nebo -1 při chybě.
 */
int get_priority(const char operator);

/**
 * \brief Funkce přiřadí aritmetickému operátoru odpovídající funkci.
 * \param operator Znak, který obsahuje daný aritmetický operátor.
 * \return operation odpovídající funkce, nebo NULL při chybě.
 */
operation get_operation(const char operator);

/**
 * \brief Funkce sečtě dvě čísla s plovoucí desetinnou čárkou.
 * \param a první operand.
 * \param b druhý operand.
 * \return double výsledek operace a + b.
 */
double sum(const double a, const double b);

/**
 * \brief Funkce odečte dvě čísla s plovoucí desetinnou čárkou.
 * \param a první operand.
 * \param b druhý operand.
 * \return double výsledek operace a - b.
 */
double sub(const double a, const double b);

/**
 * \brief Funkce vynásobí dvě čísla s plovoucí desetinnou čárkou.
 * \param a první operand.
 * \param b druhý operand.
 * \return double výsledek operace a * b.
 */
double mul(const double a, const double b);

/**
 * \brief Funkce vynásobí všechny hodnoty pole `array` konstantou `multiplier`.
 * \param array Násobené pole.
 * \param multiplier Násobící konstanta.
 * \param arr_len Délka pole.
 * \return int 1 pokud vše dopadlo dobře, jinak 0.
 */
int array_mul(double *array, const double multiplier, const size_t arr_len);

/**
 * \brief Funkce sečte dvě pole `array` a `array_2`, výsledek bude v prvním poli `array`.
 * \param array První sčítané pole, nakonci v něm bude výsledek.
 * \param array_2 Druhé sčítané pole.
 * \param arr_len Délka pole.
 * \return int 1 pokud vše dopadlo dobře, jinak 0.
 */
int array_sum(double *array, const double *array_2, const size_t arr_len);

/**
 * \brief Funkce zjistí, zda je předaný znak `symbol` aritmetický operátor.
 * \param symbol Znak aritmetického operátoru.
 * \return int 1 pokud je, pokud není 0.
 */
int is_operator(const char symbol);

#endif