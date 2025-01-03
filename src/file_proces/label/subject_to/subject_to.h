/**
 * \file subject_to.h
 * \author Antonín Hobl (hobl@students.zcu.cz)
 * \brief Hlavičkový soubor s deklaracemi funkcí nutných pro zpracování sekce Subject To v vstupním souboru.
 * \version 1.0
 * \date 2025-01-02
 */

#ifndef SUBJECT_TO_H
#define SUBJECT_TO_H

#include "../function/function.h"

#define TEXT_SPLITTER ":"
#define DOUBLE_DOT ':'

#define EQUAL_LEN 1

/** \brief Definice struktury, ve které budou uloženy všechny potřebná data ze sekce Subject To*/
struct subject_to{
    double **coefs;
    size_t line_count;
    char *operators;
    double *right_sides;
};

/**
 * \brief Funkce zpracuje všechny řádky ze sekce Subject To pomocí funkce `parse_equation`a vrátí dynamicky alokovanou
 * strukturu `subject_to` alokovanou pomocí funkce `subject_to_alloc`.
 * \param subject_to Pole řetězců, které rezprezentuje řádky sekce bounds, jenž budou zpracovány.
 * \param line_count Počet řádek sekce Subject To.
 * \param generals Ukazatel na instanci struktury `generals`, potřebnout kvůli kvůli informacím o proměnných. 
 * \return subject_to* Ukazatel na dynamicky alokovanou instanci struktury `subject_to`, obsahující zpracovaná data, nebo `NULL` při chybě.
 */
struct subject_to *process_subject_to(char **subject_to, const size_t line_count, const struct generals *generals);

/**
 * \brief Funkce zpracuje rovnici, nebo nerovnici. Funkce zpracuje aritmetický výraz na jedné straně a uloží koeficienty proměnných 
 * do vnitřního pole struktury `subject_to` pomocí funkce `parse_artithmetic_expression`, také uloží hodnoty pravých stran do dalšího 
 * vnitřního pole struktury `subject_to`.
 * \param subject_to Ukazatel na dynamicky alokovanou instanci struktury `subject_to`, do které se ukládají data.
 * \param equation Řetězec obsahující rovnici, která se má zpracovat.
 * \param id Identifikátor zpracovávané řádky.
 * \param generals Ukazatel na dynamicky alokovanou instanci struktury `generals`, potřebnout kvůli kvůli informacím o proměnných.
 * \return int 1 pokud vše dopadlo dobře, jinak 0.
 */
int parse_equation(struct subject_to *subject_to, char *equation, const size_t id, const struct generals *generals);

/**
 * \brief Funkce dynamicky alokuje instanci struktury `subject_to`, kterou inicializuje pomocí funkce `subject_to_init`.
 * \param coef_count Počet koeficientů proměnných.
 * \param line_count Počet rovnic.
 * \return subject_to* Ukazatel na nově dynamicky alokovanou instanci struktury `subject_to`, nebo `NULL` při chybě.
 */
struct subject_to *subject_to_alloc(const size_t coef_count, const size_t line_count);

/**
 * \brief Funkce pro inicializaci instance struktury `subject_to`.
 * \param subject_to Ukazatel na inicializovanou instanci struktury `subject_to`.
 * \param coef_count Počet koeficientů proměnných.
 * \param line_count Počet rovnic.
 * \return int 1, pokud inicializace subject_to proběhla v pořádku, jinak 0.
 */
int subject_to_init(struct subject_to *subject_to, const size_t coef_count, const size_t line_count);

/**
 * \brief Funkce dynamicky alokuje jednotlivé řádky v matici koeficientů proměnných `coefs`.
 * \param coefs Matice, ve které se alokojí jednotlivé řádky
 * \param coef_count Počet koeficientů proměnných.
 * \param line_count Počet řádek matice.
 * \return int 1, pokud alokace dopadla dobře, jinak 0.
 */
int coefs_alloc(double **coefs, const size_t coef_count, const size_t line_count);

/**
 * \brief Funkce uvolní dynmicky alokované řádky v matici koeficientů proměnných `coefs`.
 * \param coefs Uvolňovaná matice.
 * \param line_count Počet řádek matice.
 */
void coefs_dealloc(double **coefs, const size_t line_count);

/**
 * \brief Funkce provede korektní uvolnění členů instance struktury `subject_to`.
 * \param function Ukazatel na instanci struktury `subject_to` jejichž členy budou korektně uvolněny.
 */
void subject_to_deinit(struct subject_to *subject_to);

/**
 * \brief Funkce uvolní dynamicky alokovanou instanci struktury `subject_to`. K uvolnění vnitřních členů
 *        struktury používá funkce `subject_to_deinit`. Ukazatel, na který `subject_to` ukazuje bude nastaven na
 *        hodnotu `NULL`, tzn. neplatný ukazatel.
 * \param function Ukazatel na ukazatel na instanci struktury `subject_to`, která bude uvolněna.
 */
void subject_to_dealloc(struct subject_to **subject_to);

/**
 * \brief Funkce zkontroluje, zda předaný řetězec `subject_to`, ve kterém je uložena jedna rovnice ze sekce Subject To, neobsahuje zakázané znaky.
 * \param function Řetězec s rovnicí ze sekce Subject To, který se kontroluje.
 * \return int 0 pokud obsahuje zakázaný znak, jinak 1.
 */
int control_subject_to(char *subject_to);

/**
 * \brief Funkce zkopíruje členy struktury `smaller_subject_to` do struktury s větším vnitřním polem `subject_to` s předpokladem stejným počtem řádek.
 * \param subject_to Ukazatel na dynamicky ulokovanou instanci struktury `subject_to`, do které se budou kopírovat vnitřní členy menší struktury.
 * \param smaller_subject_to Ukazatel na dynamicky ulokovanou instanci struktury `subject_to`, ze které se budou kopírovat členy do větší struktury.
 * \param var_count_bigger Počet proměnných větší struktury
 * \param var_count_smaller Počet proměnných menší struktury
 * \return int 0 pokud vše dopadlo dobře, jinak 1.
 */
int copy_sub_matrix(struct subject_to *subject_to, const struct subject_to *smaller_subject_to, const size_t var_count_bigger, const size_t var_count_smaller);

/**
 * \brief Funkce zkontroluje syntaxi jedné řádky sekce Subject To.
 * \param line Řetězec obsahující kontrolovanou řádku.
 * \return int 1 pokud není syntaktická chyba, 0 pokud je.
 */
int check_line_syntax(const char *line);

/**
 * \brief Funkce prohodí znaménka v předaném poli `coefs`.
 * \param coefs Pole ve kterém se prohodí znaménka.
 * \param var_count Délka předaného pole.
 */
void switch_signs(double *coefs, const size_t var_count);

#endif