/**
 * \file simlex.h
 * \author Antonín Hobl (hobl@students.zcu.cz)
 * \brief Hlavičkový soubor s deklaracemi funkcí pro úspěšné provedení dvoufázového simplexového algoritmu.
 * \version 1.0
 * \date 2025-01-03
 */

#ifndef SIMPLEX_H
#define SIMPLEX_H

#include "../file_proces/file_proces.h"

#define DEFALUT_SLACK_VAR_VALUE 1.0
#define DEFALUT_SURPLEX_VAR_VALUE -1.0
#define GRATER_ADD_VAR_COUNT 2

#define TABLE_ROW_COL_CONST 1

/** \brief Definice struktury reprezentující simplexovou tabulku*/
struct simplex_table{
    double **coefs;
    size_t *base;
    size_t rows;
    size_t cols;
    size_t *artificial_vars_ids;
    size_t artificial_vars_count;
};

/**
 * \brief Funkce provede dvoufázový simplexový algoritmus pomocí dalších funkcí a vypíše výsledek pomocí funkce `print_solution`.
 * \param processed_file Zpracovaný soubor nutný pro vytvoření simplexové tabulky.
 * \param output_file_name Řetězec obsahující cestu k výstupnímu souboru, pokud byla zadána.
 * \return int 1, pokud vše dopadlo dobře, jinak 0.
 */
int simplex(struct processed_file *processed_file, const char *output_file_name);

/**
 * \brief Funkce prvádí iterace simplexové tabulky `table`.
 * \param table Ukazatel na instanci struktury `simplex_table`, která se iteruje a upravuje.
 * \return int 1, pokud vše dopadlo dobře, jinak 0.
 */
int iterate(struct simplex_table *table);

/**
 * \brief Funkce přidá přidatné proměnné.
 * \param subject_to Ukazatel na instanci struktury `subject_to`, kvůli koeficientům původních proměnných.
 * \param generals Ukazatel na instanci struktury `generals`, kvůli informacím o původních proměnných.
 * \return simplex_table* Ukazatel na dynamicky alokovanou instanci struktury `simplex_table`, nebo `NULL` při chybě.
 */
struct simplex_table *add_variables(const struct subject_to *subject_to, const struct generals *generals);

/**
 * \brief Funkce nastaví koeficienty přidatných proměnných a alokuje simplexovou tabulku i s přidatnými proměnnými pomocí funkce `simplex_table_alloc`.
 * \param help_subject_to Ukazatel na instanci struktury `subject_to` zvětšené o počet přidatných proměnných, jejichž koefocoenty se naplní.
 * \param help_function Ukazatel na instanci struktury `function` zvětšené o počet přidatných proměnných, jejichž koefocoenty se naplní.
 * \param generals Ukazatel na instanci struktury `generals`, kvůli informacím o původních proměnných.
 * \param additional_variables_count Počet přidatných proměnných
 * \return simplex_table* Ukazatel na dynamicky alokovanou instanci struktury `simplex_table`, nebo `NULL` při chybě.
 */
struct simplex_table *set_additional_variables(struct subject_to *help_subject_to, struct function *help_function, const struct generals *generals, const size_t additional_variables_count);

/**
 * \brief Funkce spočítá počet přidatných proměnných.
 * \param subject_to Ukazatel na instanci struktury `subject_to`, ze které to funkce sočítá
 * \return size_t počet přidatných proměnných.
 */
size_t count_additional_variables(const struct subject_to *subject_to);

/**
 * \brief Funkce dynamicky alokuje instanci struktury `simplex_table`, kterou inicializuje pomocí funkce `simplex_table_init`.
 * \param function Ukazatel na instanci struktury `function` obsahující koeficienty účelové funkce.
 * \param subject_to Ukazatel na instanci struktury `subject_to` obsahující koeficienty omezení.
 * \param var_count Celkový počet proměnných
 * \param base_ids Pole obsahující identifikátory bázových proměnných.
 * \param artificial_vars_ids Pole obsahující identifikátory umělých proměnných.
 * \param artificial_vars_count Počet umělých proměnných.
 * \return simplex_table* Ukazatel na nově dynamicky alokovanou instanci struktury `simplex_table`, nebo `NULL` při chybě.
 */
struct simplex_table *simplex_table_alloc(const struct function *function, const struct subject_to *subject_to, size_t var_count, const size_t *base_ids, const size_t *artificial_vars_ids, size_t artificial_vars_count);

/**
 * \brief Funkce pro inicializaci instance struktury `simplex_table`.
 * \param bounds Ukazatel na inicializovanou instanci struktury `simplex_table`.
 * \param function Ukazatel na instanci struktury `function` obsahující koeficienty účelové funkce.
 * \param subject_to Ukazatel na instanci struktury `subject_to` obsahující koeficienty omezení.
 * \param var_count Celkový počet proměnných
 * \param base_ids Pole obsahující identifikátory bázových proměnných.
 * \param artificial_vars_ids Pole obsahující identifikátory umělých proměnných.
 * \param artificial_vars_count Počet umělých proměnných.
 * \return int 1, pokud inicializace simplex_table proběhla v pořádku, jinak 0.
 */
int simplex_table_init(struct simplex_table *table, const struct function *function, const struct subject_to *subject_to, size_t var_count, const size_t *base_ids, const size_t *artificial_vars_ids, size_t artificial_vars_count);

/**
 * \brief Funkce dynamicky alokuje řádky matice simplexové tabulky.
 * \param coefs Matice siplexové tabulky, ve které se alokují řádky
 * \param rows Počet řádek matice.
 * \param cols Počet sloupců matice
 * \return int 1, pokud alokace dopadla dobře, jinak 0.
 */
int rows_alloc(double **coefs, const size_t rows, const size_t cols);

/**
 * \brief Funkce uvolní dynamicky alokované řádky matice a nasví jejich ukazatel na hodnotu `NULL`, tzn. neplatný ukazatel..
 * \param coefs Matice, jejíž řádky se uvolní.
 * \param rows Počet řádek matice.
 */
void rows_dealloc(double **coefs, const size_t rows);

/**
 * \brief Funkce provede korektní uvolnění členů instance struktury `simplex_table`.
 * \param table Ukazatel na instanci struktury `simplex_table` jejichž členy budou korektně uvolněny.
 */
void simplex_table_deinit(struct simplex_table *table);

/**
 * \brief Funkce uvolní dynamicky alokovanou instanci struktury `simplex_table`. K uvolnění vnitřních členů
 *        struktury používá funkce `simplex_table_deinit`. Ukazatel, na který `simplex_table` ukazuje bude nastaven na
 *        hodnotu `NULL`, tzn. neplatný ukazatel.
 * \param table Ukazatel na ukazatel na instanci struktury `simplex_table`, která bude uvolněna.
 */
void simplex_table_dealloc(struct simplex_table **table);

/**
 * \brief Funkce nastaví v řádce s účelovou funkcí simplexové tabulby koeficienty bázových proměnných na hodnotu `0`.
 * \param table Ukazatel na instanci struktury `simplex_table`, kde se budou koeficienty nulovat.
 * \return int 1 pokud vše dopadlo dobře, jinak 0.
 */
int set_base_zero(struct simplex_table *table);

/**
 * \brief Funkce sečte řádek s id `src_row` vynásobený konstanou `multiplier` k řádku s id `dst_row`.
 * \param table Ukazatel na instanci struktury `simplex_table`, kde se budou sčítat řádky.
 * \param src_row Id prvního řádku, který se bude přičítat.
 * \param dst_row Id druhéhé řádku, ke kterému bude přičítán první řádek.
 * \param multiplier Konstanta, kterou se vynásobí první řádek `src_row`.
 * \return int 1 pokud vše dopadlo dobře, jinak 0.
 */
int sum_rows(struct simplex_table *table, const size_t src_row, const size_t dst_row, const double multiplier);

/**
 * \brief Funkce zkontroluje, zda se v řádku s účelovou funkcí nenachází záporná hodnota.
 * \param table Ukazatel na instanci struktury `simplex_table`, ve které se kontroluje řádku s účelovou funkcí.
 * \return int 1, pokud se v řádce nachází záporná hodnota, 0 pokud ne, jinak -1 při chybě.
 */
int check_non_negative_row(const struct simplex_table *table);

/**
 * \brief Funkce získá pivotní sloupec ze siplexové tabulky `table`.
 * \param table Ukazatel na instanci struktury `simplex_table`, ve které se hledá pivotní sloupec.
 * \return int id pivotního sloupce, jinak -1 při chybě.
 */
int get_pivot_col(struct simplex_table *table);

/**
 * \brief Funkce vydělí řádku s předaným id `row_id` konstantou `divisor`.
 * \param table Ukazatel na instanci struktury `simplex_table`, ve které se bude dělit řádka.
 * \param row_id Identifikátor řádky, která se budě dělit.
 * \param divisor Konstanta, kterou se bude řádka dělit.
 * \return int 1 pokude dělení proběhlo v pořádku, jinak 0.
 */
int divide_row(struct simplex_table *table, const size_t row_id, const double divisor);

/**
 * \brief Funkce vymění bázovou proměnnou za jinou ekvivalentním zkpůsobem jako simplexový algoritmus.
 * \param table Ukazatel na instanci struktury `simplex_table`, ve které mění bázová proměnná.
 * \return int 1 pokude vše proběhlo v pořádku, jinak 0.
 */
int replace_base(struct simplex_table *table);

/**
 * \brief Funkce zkontroluje, zda proměnná s předaným id `id` je umělá proměnná.
 * \param id Identifikátor proměnné, u které se rozhoduje, zda je umělá.
 * \param artificial_variables_ids Pole s Identifikátory umělých proměnných.
 * \param artificial_variables_count Počet umělých proměnných.
 * \return int 1, pokud je umělá proměnná, jinak 0.
 */
int is_artificial(const size_t id, const size_t *artificial_variables_ids, const size_t artificial_variables_count);

/**
 * \brief Funkce zkontroluje, zda nejsou v bázi tabulky umělé proměnné.
 * \param table Ukazatel na instanci struktury `simplex_table`, ve které se kontroluje báze.
 * \return int 0, pokud je v bázi umělá proměnná, 1 pokud není.
 */
int check_artificial_variables(const struct simplex_table *table);

/**
 * \brief Funkce inicializuje simplexovou tabulku `table` na druhou fázi simplexového algoritmu.
 * \param table Ukazatel na instanci struktury `simplex_table`, která bude inicializována na druhou fázi.
 * \param original_function Ukazatel na instanci struktury `function` obsahující původní koeficienty proměnných, které se korekně zkopírují do simplexové tabulky.
 * \param generals Ukazatel na instanci struktury `generals` obsahující informace o proměnných.
 * \return int 1 pokud vše dopadlo dobře, jinak 0.
 */
int phase_two_init(struct simplex_table *table, struct function *original_function, const struct generals *generals);

/**
 * \brief Funkce zkontroluje zda je proměnná s id `id` v bázi simplexové tabulky `table` a naplní ukazatel `row` indexem řádky na kterém se bázová proměnná nachází.
 * \param table Ukazatel na instanci struktury `simplex_table`, kde se bude určovat, zda proměnná patří do její báze.
 * \param id Identifikátor proměnné, u které se určuje, zda patří do báze.
 * \param row Ukazatel, který se naplní indexem řádky proměnné, pokud je v bázi.
 * \return int 1 pokud je v bázi, jinak 0.
 */
int is_in_base(const struct simplex_table *table, const size_t id, size_t *row);

/**
 * \brief Funkce zkontroluje zda výsledek splňuje omezení Bounds.
 * \param table Ukazatel na instanci struktury `simplex_table`, kde se nachází výsledek.
 * \param generals Ukazatel na instanci struktury `generals`, kde se nachází informace o proměnných.
 * \param bounds Ukazatel na instanci struktury `bounds`, kde se nachází omezení proměnných.
 * \return int 1 pokud splňuje omezení, jinak 0.
 */
int check_solution(const struct simplex_table *table, const struct generals *generals, const struct bounds *bounds);

/**
 * \brief Funkce vypíše výsledek simplexového algoritmu do výstupního souboru, pokud byl zadán, jinak vypíše výsledek do konzole.
 * \param table Ukazatel na instanci struktury `simplex_table`, kde se nachází výsledek.
 * \param generals Ukazatel na instanci struktury `generals`, kde se nachází informace o proměnných.
 * \param output_file_path Řetězec osahující cestu k výstupnímu souboru, nebo `NULL`, pokud nebyla zadána.
 * \return int 1 pokud vše dopadlo dobře, jinak 0.
 */
int print_solution(const struct simplex_table *table, const struct generals *generals, const char *output_file_path);

#endif