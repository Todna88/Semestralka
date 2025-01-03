/**
 * \file bounds.h
 * \author Antonín Hobl (hobl@students.zcu.cz)
 * \brief Hlavičkový soubor s deklaracemi funkcí nutných pro zpracování sekce Bounds v vstupním souboru.
 * \version 1.0
 * \date 2025-01-01
 */

#ifndef BOUNDS_H
#define BOUNDS_H

#include "../function/function.h"

#define BOUNDS_ARRAY_COUNT 3
#define VALUES_ARRAY_COUNT 2

#define SPACE " "

#define NAN 0.0 / 0.0
#define MINUSINF -1.0 / 0.0
#define PLUSINF 1.0 / 0.0
#define MINUSINF_LONG -4503599627370496
#define PLUSSINF_LONG 9218868437227405312
#define INFINITY_STR "Infinity"
#define INF_STR "Inf"

#define ERROR_VAR_ID -1

/** \brief Definice unionu pro přístup k hodnotě nekonečno z datového typu long int */

union Infinity{
    double d;
    long l;
};

/** \brief Definice struktury, ve které budou uloženy všechny potřebná data ze sekce Bounds*/

struct bounds{
    char **bounds;
    double **values;
};

/**
 * \brief Funkce zpracuje všechny řádky ze sekce Bounds pomocí funkce `parse_bound`a vrátí dynamicky alokovanou strukturu `bounds` alokovanou pomocí funkce `bounds_alloc`.
 * \param bounds Ukazatel na jednotlivé řádky sekce Bounds, které funkce zpracuje.
 * \param line_count Počet řádek sekce Bounds.
 * \param generals Ukazatel na instanci struktury `generals`, potřebnout kvůli obsahu počtu proměnných. 
 * \return bounds* Ukazatel na dynamicky alokovanou instanci struktury `bounds`, obsahující zpracovaná data, nebo `NULL` při chybě.
 */
struct bounds *process_bounds(char **bounds, const size_t line_count, const struct generals *generals);

/**
 * \brief Funkce zpracuje jednu řádku ze sekce Bounds a zpracovaná data zkopíruje pomocí funkce `cpy_values` do struktury `bounds`.
 * \param bounds Ukazatel na instanci struktury `bounds`, do které se uloží data.
 * \param line Zpracovávaná řádka.
 * \param generals Ukazatel na instanci struktury `generals`, potřebnout kvůli obsahu počtu proměnných.
 * \return int 1, pokud vše dopadlo dobře, jinak 0.
 */
int parse_bound(struct bounds *bounds, char *line, const struct generals* generals);

/**
 * \brief Funkce zkopíruje data z daného pole do struktury `bounds` pro jednu proměnnou danou jejím id.
 * \param bounds Ukazatel na instanci struktury `bounds`, do které se hodnoty zpokírují.
 * \param values Pole, ze kterého se bude kopírovat.
 * \param variable_id Identifikátor proměnné, které se data týkají.
 */
void cpy_values(const struct bounds *bounds, const double *values, const int variable_id);

/**
 * \brief Funkce zkontroluje syntaxi pro jednu řádku v sekci Bounds.
 * \param variable_id Identifikátor proměnné nacházející se na dané řádnce.
 * \param values Pole, ve kterém jsou uloženy hodnoty vzahující se k omezujícím operátorům dané proměnné.
 * \param const Pole jednotlivých omezujících operáturů dané proměnné.
 * \param values_len Délka pole values.
 * \return int 1 pokud není syntaktická chyba, 0 pokud je
 */
int check_syntax(const int variable_id, const double *values, const char *constr, const size_t values_len);

/**
 * \brief Funkce vyparsuje název proměnné z řetězce `token` a uloží její id do ukazalete `variable_id`.
 * \param variable_id Ukazatel na identifikátor vyparsované proměnné, který funkce naplní.
 * \param equal_flag Ukazatel na hodnotu typu size_t, obsahující informaci, zda se v řádce vyskutnul operátor =
 * \param constr Pole jednotlivých omezujících operáturů dané proměnné.
 * \param token Řetězec, který by měl obsahovat název proměnné
 * \param i Hodnota obsahující kolikátá iterace v parsování dané řadky právě probíhá.
 * \param generals Ukazatel na instanci struktury `generals`, potřebou k získání identifikátoru proměnné,
 * \param values Pole, ve kterém jsou uloženy hodnoty vzahující se k omezujícím operátorům dané proměnné.
 * \return int 1 pokud vše dopadlo dobře, jinak 0
 */
int parse_var(int *variable_id, size_t *equal_flag, char *constr, const char *token, const int i, const struct generals *generals, double *values);

/**
 * \brief Funkce zjistý daný omezující operátor vztahující se k dané proměné a uloží ho do pole `constr`.
 * \param last_char Znak, který by měl obsahovat jeden ze známých porovnávacích operátorů.
 * \param constr Pole, do kterého se daný operátor uloží.
 * \param i Honodta obsahující kolikátá iterace v parsování dané řadky právě probíhá.
 * \param equal_flag Ukazatel na hodnotu typu size_t, obsahující informaci, zda se v řádce vyskutnul operátor =
 * \param variable_id Ukazatel na identifikátor proměnné, ke které se vztahuje daný operátor, pokud byl již zjištěn.
 * \return int 1 pokud vše dopadlo dobře, jinak 0
 */
int parse_oper(char last_char, char *constr, const int i, size_t *equal_flag, const int variable_id);

/**
 * \brief Funkce vyparsuje omezující hodnotu z řetězce `token` a uloží jej do pole `values`.
 * \param variable_id Identifikátor proměnné, ke které se vztahuje daná hodnota.
 * \param equal_flag Ukazatel na hodnotu typu size_t, obsahující informaci, zda se v řádce vyskutnul operátor =
 * \param values Pole, do kterého se hodnota uloží.
 * \param constr Pole jednotlivých omezujících operáturů dané proměnné.
 * \param token Řetězec, který by měl obsahovat omezující hodnotu.
 * \param i Honodta obsahující kolikátá iterace v parsování dané řadky právě probíhá.
 * \return int 1 pokud vše dopadlo dobře, jinak 0
 */
int parse_num(const int variable_id, size_t *equal_flag, double *values, char *constr, const char *token, const int i);

/**
 * \brief Funkce zkontoluje zda předaný znak `operator` je jeden z omezujících operátorů.
 * \param operator Znak, který se kontroluje.
 * \return int 1 pokud je to omezující operátor, 0 pokud není
 */
int check_operator(const char operator);

/**
 * \brief Funkce prohodí omezující operátor, na který ukazuje ukazatel `operator` za opačný.
 * \param operator Ukazatel na omezující operátor, který se prohodí.
 */
void switch_operator(char *operator);

/**
 * \brief Funkce vyparsuje řádku `line`, pokud je proměná na řádce bez omezení (free) a uloží potřebné hodonty do struktury `bounds` pomocí funkce `set_free_var`.
 * \param bounds Ukazatel na instanci struktury `bounds`, do které se uloží potřebné hodnoty.
 * \param line Řetězec, ve ketrém je uložen obsah řádky.
 * \param generals Ukazatel na instanci struktury `generals`, potřebou k získání identifikátoru proměnné,
 * \return int 1 pokud vše dopadlo dobře, jinak 0
 */
int parse_free(struct bounds *bounds, char *line, const struct generals* generals);

/**
 * \brief Funkce nastaví hodnoty ve struktuře `bounds`, pokud je proměnná bez omezení (free).
 * \param bounds Ukazatel na instanci struktury `bounds`, do které se uloží potřebné hodnoty.
 * \param variable_id Hodnota obsahující identifikátor dané proměnné.
 * \return int 1 pokud vše dopadlo dobře, jinak 0
 */
int set_free_var(struct bounds *bounds, const int variable_id);

/**
 * \brief Funkce zjistí zda předaný znak `symbol` je začátek čísla.
 * \param symbol Znak, u kterého funkce zjišťuje, zda je začátek čísla
 * \return int 1 pokud je to začátek čísla, 0 pokud není
 */
int is_number(const char symbol);

/**
 * \brief Funkce dynamicky alokuje instanci struktury `bounds`, kterou inicializuje pomocí funkce `bounds_init`.
 * \param var_count Počet všech známých proměnných.
 * \return bounds* Ukazatel na nově dynamicky alokovanou instanci struktury `bounds`, nebo `NULL` při chybě.
 */
struct bounds *bounds_alloc(const size_t var_count);

/**
 * \brief Funkce pro inicializaci instance struktury `bounds`.
 * \param bounds Ukazatel na inicializovanou instanci struktury `bounds`.
 * \param var_count Celkový počet všech proměnných.
 * \return int 1, pokud inicializace bounds proběhla v pořádku, jinak 0.
 */
int bounds_init(struct bounds *bounds, const size_t var_count);

/**
 * \brief Funkce pro alokaci vnitřních polí struktury `bounds`.
 * \param bounds Ukazatel na instanci struktury `bounds`, ve které se budou alokovat pole.
 * \param var_count Celkový počet všech proměnných.
 * \return int 1, pokud alokace polí proběhla v pořádku, jinak 0.
 */
int arrays_alloc(struct bounds *bounds, const size_t var_count);

/**
 * \brief Funkce uvolní dynamicky alokovaná vnitřní pole strktury `bounds` a jejich ukazatele nastaví na hodnotu `NULL`, tzn. neplatný ukazatel.
 * \param bounds Ukazatel na instanci struktury `bounds`, ve které se uvolí vnitřní pole.
 * \param var_count Celkový počet všech proměnných.
 */
void arrays_dealloc(struct bounds *bounds, const size_t var_count);

/**
 * \brief Funkce uvolní vnitřní pole pomocí fuknce `arrays_dealloc`, a korektně uvolní ukazatele na tyto pole.
 * \param bounds Ukazatel na instanci struktury `bounds` jejichž členy budou korektně uvolněny.
 * \param var_count Celkový počet všech proměnných.
 */
void bounds_deinit(struct bounds *bounds, const size_t var_count);

/**
 * \brief Funkce uvolní dynamicky alokovanou instanci struktury `bounds`. K uvolnění vnitřních členů
 *        struktury používá funkce `bounds_deinit`. Ukazatel, na který `bounds` ukazuje bude nastaven na
 *        hodnotu `NULL`, tzn. neplatný ukazatel.
 * \param bounds Ukazatel na ukazatel na instanci struktury `bounds`, která bude uvolněna.
 * \param var_count Celkový počet všech proměnných.
 */
void bounds_dealloc(struct bounds **bounds, const size_t var_count);

/**
 * \brief Funkce nasaví strukturu `bounds` na výchozí hodnoty, tedy že všechny proměnné jsou >= 0.
 * \param bounds Ukazatel na instanci struktury `bounds`, ve které se nastaví hodnoty.
 * \param var_count Celkový počet všech proměnných.
 */
void set_default_bounds(struct bounds *bounds, const size_t var_count);

/**
 * \brief Funkce zjsití, zda předaná hodna `value` je NaN.
 * \param value Hodnota, u které se zjišťuje zda je NaN.
 * \return int 1 pokud je NaN, 0 pokud není
 */
int is_nan(const double value);

/**
 * \brief Funkce zjsití, zda předaná hodna `value` je mínus nekonečno.
 * \param value Hodnota, u které se zjišťuje zda je mínus nekonečno.
 * \return int 1 pokud je mínus nekonečno, 0 pokud není
 */
int is_plus_inf(const double value);

/**
 * \brief Funkce zjsití, zda předaná hodna `value` je plus nekonečno.
 * \param value Hodnota, u které se zjišťuje zda je plus nekonečno.
 * \return int 1 pokud je plus nekonečno, 0 pokud není
 */
int is_minus_inf(const double value);

#endif