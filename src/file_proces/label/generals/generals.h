/**
 * \file generals.h
 * \author Antonín Hobl (hobl@students.zcu.cz)
 * \brief Hlavičkový soubor s deklaracemi funkcí nutných pro zpracování sekce Generals v vstupním souboru.
 * \version 1.0
 * \date 2025-01-02
 */

#ifndef GENERALS_H
#define GENERALS_H

#include "../label.h"

#define MAX_VAR_LEN 256

#define INDEXING_FROM_ZERO 1

#define ADD_ONE 1

/** \brief Definice struktury pro uložení názvu proměnných a jejich počtu*/

struct generals{
    char **variables;
    size_t variables_count;
};

/**
 * \brief Funkce zpracuje všechny řádky ze sekce Generals pomocí funkce `get_variables` a vrátí dynamicky alokovanou 
 * strukturu `generals` alokovanou pomocí funkce `generals_alloc`.
 * \param bounds Ukazatel na jednotlivé řádky sekce Generals, které funkce zpracuje.
 * \param line_count Počet řádek sekce Generals.
 * \return generals* Ukazatel na dynamicky alokovanou instanci struktury `generals`, obsahující zpracovaná data, nebo `NULL` při chybě.
 */
struct generals *process_generals(char **generals, size_t line_count);

/**
 * \brief Funkce vyparsuje všechny názvy proměnných z řetězce `line`. Dále pro ně alokuje místo a uloží je do struktury `generals` pomocí funkce `variables_alloc`.
 *  a postupně inktementuje počet proměnných;
 * \param line Řetězec obsahující názvy proměnných oddělené mezerou.
 * \param generals Dynamicky alokovaná instance struktury `generals` do níž se uloží názvy proměnných a jejich počet.
 * \return int 1 pokud vše dopadlo dobře, jinak 0.
 */
int get_variables(char *line, struct generals *generals);

/**
 * \brief Funkce dynamicky alokuje instanci struktury `generals`, kterou inicializuje pomocí funkce `generals_init`.
 * \return generals* Ukazatel na nově dynamicky alokovanou instanci struktury `generals`, nebo `NULL` při chybě.
 */
struct generals *generals_alloc();

/**
 * \brief Funkce pro inicializaci instance struktury `generals`.
 * \param generals Ukazatel na inicializovanou instanci struktury `generals`.
 * \return int 1, pokud inicializace generals proběhla v pořádku, jinak 0.
 */
int generals_init(struct generals *generals);

/**
 * \brief Funkce uvolní dynamicky alokovanou instanci struktury `generals`. K uvolnění vnitřních členů
 *        struktury používá funkce `generals_deinit`. Ukazatel, na který `generals` ukazuje bude nastaven na
 *        hodnotu `NULL`, tzn. neplatný ukazatel.
 * \param generals Ukazatel na ukazatel na instanci struktury `generals`, která bude uvolněna.
 */
void generals_dealloc(struct generals **generals);

/**
 * \brief Funkce provede korektní uvolnění členů instance struktury `generals`.
 * \param s Ukazatel na instanci struktury `generals` jejichž členy budou korektně uvolněny.
 */
void generals_deinit(struct generals *generals);

/**
 * \brief Funkce dynamicky alokuje místo pro název proměnné, kterou inicializuje pomocí funkce `variable_init`.
 * \param var_len Délka názvu proměnné i s ukončovacím znakem.
 * \param var Řetězec s obsahem názvu proměnné.
 * \return char* Ukazatel na nově dynamicky alokovaný název proměnné, nebo `NULL` při chybě.
 */
char *variable_alloc(const size_t var_len, const char *var);

/**
 * \brief Funkce pro inicializaci názvu proměnné.
 * \param new_var Řetězec obsahující inicializovaný název proměnné.
 * \param var Řetězec s obsahem názvu proměnné.
 * \param var_len Délka názvu proměnné i s ukončovacím znakem.
 * \return int 1, pokud inicializace zásobníku proběhla v pořádku, jinak 0.
 */
int variable_init(char *new_var, const char *var, const size_t var_len);

/**
 * \brief Funkce realokuje vnitřní pole struktury `generals` pro novou proměnnou, naalokuje pro ni místo pomocí 
 * funkce `variable_alloc` a uloží nové pole zpět do generals pomocí funkce `generals_set_variables`.
 * \param generals Ukazatel na instanci struktury `generals`, ve které bude nově realokované pole s názvem nové proměnné.
 * \param var_len Délka názvu proměnné i s ukončovacím znakem.
 * \param variable Řetězec obsahující název nové proměnné.
 * \return int 1, pokud vše dopadlo v pořádku, jinak 0.
 */
int variables_alloc(struct generals *generals, const size_t var_len, const char *variable);

/**
 * \brief Funkce uvolní dynamicky alokovaný řetězec s názvem proměnné. Řetězec, na který `variable` ukazuje bude nastaven na
 *        hodnotu `NULL`, tzn. neplatný ukazatel.
 * \param variable Ukazatel na řetězec, který bude uvolněn.
 */
void variable_dealloc(char **variable);

/**
 * \brief Funkce uvolní dynamicky alokované pole řetězců. K uvolnění jednotlivých řetězců
 * v poli se použije funkce `variable_dealloc`. Pole, na které `variables` ukazuje bude nastaven na
 * hodnotu `NULL`, tzn. neplatný ukazatel.
 * \param variable Ukazatel na pole řetězců, které bude uvolněno.
 * \param var_count Počet proměnných, tedy délka pole.
 */
void variables_dealloc(char ***variables, const size_t var_count);

/**
 * \brief Funkce předá do struktury `generals` nově realokované pole proměnných.
 * \param generals Ukazatel na instanci struktury `generals`, ve které bude nové pole proměnných.
 * \param variables Nové pole proměnných.
 * \return int 1, pokud vše dopadlo v pořádku, jinak 0.
 */
int generals_set_variables(struct generals *generals, char **variables);

/**
 * \brief Funkce zjistí id předané proměné `var_1` ve vnitřním poli struktury `generals`.
 * \param var Řetězec obsahující název proměnné
 * \param generals Ukazatel na instanci struktury `generals`, která obsahuje pole proměnných.
 * \return int id proměnné v poli, nebo -1 pokud proměnná v poli není.
 */
int get_variable(const char *var_1, const struct generals *generals);

/**
 * \brief Funkce zjistí zda předaný řetězec `line` obsahuje nějaký z názvu proměnných.
 * \param line Řetězec ve kterém se hledá název kterékoliv z proměnných
 * \param generals Ukazatel na instanci struktury `generals`, která obsahuje pole proměnných.
 * \return int 1, pokud obsahuje, jinak 0.
 */
int search_variables(char *line, const struct generals *generals);

/**
 * \brief Funkce zkontroluje zda název proměnné neobsahuje některý ze zakázaných znaků, nebo jestli nezačíná číslem.
 * \param variable_name Řetězec s názvem proměnná, jejíž název se bude kontrolovat.
 * \return int 1, pokud je název v pořádku, jinak 0.
 */
int check_variable_name(char *variable_name);

#endif