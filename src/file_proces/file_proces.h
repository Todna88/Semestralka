/**
 * \file file_proces.h
 * \author Antonín Hobl (hobl@students.zcu.cz)
 * \brief Hlavičkový soubor s deklaracemi funkcí pro rozdělení souboru podle sekcí.
 * \version 1.0
 * \date 2025-01-03
 */

#ifndef FILE_PROCES_H
#define FILE_PROCES_H

#include "label/generals/generals.h"
#include "label/function/function.h"
#include "label/subject_to/subject_to.h"
#include "label/bounds/bounds.h"

#define MAX_LINE_COUNT 1

/** \brief Definice struktury pro uložení zpracovaného souboru*/
struct processed_file{
    struct generals *generals;
    struct function *function;
    struct subject_to *subject_to;
    struct bounds *bounds;
};

/** \brief Definice struktury pro uložení rozděleného souboru podle sekcí*/
struct divided_file{
    char **generals;
    size_t generals_line_count;
    char **bounds;
    size_t bounds_line_count;
    char *function;
    char *function_type;
    char **subject_to;
    size_t subject_to_line_count;
};

#define DEFAUlT_DIVIDED_FILE {NULL, 0, NULL, 0, NULL, NULL, NULL, 0}

/**
 * \brief Funkce rozdělí soubor `file` podle sekcí pomocí funkce `divide_file`, následně jednotlivé sekce zpracuje pomocí dalších funkcí 
 * a uloží jej zpracované do struktury `processed_file`.
 * \param file Soubor, který se rozdělí a zpracuje.
 * \return processed_file* Ukazatel na dynamicky alokovanou instanci struktury `processed_file`, obsahující zpracovaná data, nebo `NULL` při chybě.
 */
struct processed_file *process_file(FILE *file);

/**
 * \brief Funkce rozdělí soubor `file` pomocí dalších funkcí a uloží do dynamicky alokované struktury `divided_file`, kterou alokuje pomocí funkce `divided_file_alloc()`.
 * \param file Soubor, který se rozdělí.
 * \return divided_file* Ukazatel na dynamicky alokovanou instanci struktury `divided_file`, obsahující rozdělený soubor podle sekcí, nebo `NULL` při chybě.
 */
struct divided_file *divide_file(FILE *file);

/**
 * \brief Funkce dynamicky alokuje instanci struktury `divided_file`, kterou inicializuje pomocí funkce `divided_file_init`.
 * \return divided_file* Ukazatel na nově dynamicky alokovanou instanci struktury `divided_file`, nebo `NULL` při chybě.
 */
struct divided_file *divided_file_alloc();

/**
 * \brief Funkce pro inicializaci instance struktury `divided_file`.
 * \param divided_file Ukazatel na inicializovanou instanci struktury `divided_file`.
 * \return int 1, pokud inicializace bounds proběhla v pořádku, jinak 0.
 */
int divided_file_init(struct divided_file *divided_file);

/**
 * \brief Funkce dynmaicky realokuje řádky ve vnitřním poli struktury `divided_file`, přidá novou řádku pomocí funkce `line_alloc` pro 
 * sekci Subject to a aktualizuje jejich počet.
 * \param file Ukazatel na dynamicky alokovanou strukturu `divided_file`, v níž se realokuje pole pro novou řádku sekce Subject To a zvýší je jejich počet.
 * \param line_len Délka nové řádky.
 * \param line_count Nový počet řádek.
 * \param line Řetězec obsahující novou řádku.
 * \return int 1 pokud vše dopadlo dobře, jinak 0.
 */
int subj_to_alloc(struct divided_file *file, const size_t line_len, const size_t line_count, const char *line);

/**
 * \brief Funkce dynmaicky alokuje místo pro řádku s účelovou funkcí pomocí funkce `line_alloc` a uloží, jestli se mí funkce maximalizovat, nebo minimalizovat.
 * \param file Ukazatel na dynamicky alokovanou strukturu `divided_file`, v níž se alokuje řádka pro účelovou funkci.
 * \param line_len Délka řádky.
 * \param function_type Řetězec obsahující zda se má funkce maximalizovat, nebo minimalizovat.
 * \param line Řetězec obsahující řádek s účelovou funkcí.
 * \return int 1 pokud vše dopadlo dobře, jinak 0.
 */
int func_alloc(struct divided_file *file, const size_t line_len, char *function_type, const char *line);

/**
 * \brief Funkce dynmaicky realokuje řádky ve vnitřním poli struktury `divided_file`, přidá novou řádku pomocí funkce `line_alloc` pro 
 * sekci Bounds a aktualizuje jejich počet.
 * \param file Ukazatel na dynamicky alokovanou strukturu `divided_file`, v níž se realokuje pole pro novou řádku sekce Bounds a zvýší je jejich počet.
 * \param line_len Délka nové řádky.
 * \param line_count Nový počet řádek.
 * \param line Řetězec obsahující novou řádku.
 * \return int 1 pokud vše dopadlo dobře, jinak 0.
 */
int bound_alloc(struct divided_file *file, const size_t line_len, const size_t line_count, const char *line);

/**
 * \brief Funkce dynmaicky realokuje řádky ve vnitřním poli struktury `divided_file`, přidá novou řádku pomocí funkce `line_alloc` pro 
 * sekci Generals a aktualizuje jejich počet.
 * \param file Ukazatel na dynamicky alokovanou strukturu `divided_file`, v níž se realokuje pole pro novou řádku sekce Generals a zvýší je jejich počet.
 * \param line_len Délka nové řádky.
 * \param line_count Nový počet řádek.
 * \param line Řetězec obsahující novou řádku.
 * \return int 1 pokud vše dopadlo dobře, jinak 0.
 */
int gen_alloc(struct divided_file *file, const size_t line_len, const size_t line_count, const char *line);

/**
 * \brief Funkce dynamicky alokuje místo pro řetězec (řádku), který inicializuje pomocí funkce `line_init`.
 * \param line_len Délka řetězce.
 * \param line Řetězec s obsahem nového řetězce.
 * \return char* Nově dynamicky alokovaný řetězec, nebo `NULL` při chybě.
 */
char *line_alloc(const size_t line_len, const char *line);

/**
 * \brief Funkce pro inicializaci nového řetězce.
 * \param new_line Inicializovaný řetězec.
 * \param line Řetězec s obsahem nového řetězce.
 * \param line_len Délka nového řetězce
 * \return int 1, pokud inicializace řetězce proběhla v pořádku, jinak 0.
 */
int line_init(char *new_line, const char *line, const size_t line_len);

/**
 * \brief Funkce uvolní dynamicky alokovaný řetězec. Řetězec, na který `function` ukazuje bude nastaven na
 *        hodnotu `NULL`, tzn. neplatný ukazatel.
 * \param line Ukazatel na řetězec, který bude uvolněna.
 */
void line_dealloc(char **line);

/**
 * \brief Funkce uvolní dynamicky alokovanou instanci struktury `divided_file`. K uvolnění vnitřních členů
 *        struktury používá funkce `divided_file_deinit`. Ukazatel, na který `divided_file` ukazuje bude nastaven na
 *        hodnotu `NULL`, tzn. neplatný ukazatel.
 * \param divided_file Ukazatel na ukazatel na instanci struktury `divided_file`, která bude uvolněna.
 */
void divided_file_dealloc(struct divided_file **divided_file);

/**
 * \brief Funkce provede korektní uvolnění členů instance struktury `divided_file`.
 * \param function Ukazatel na instanci struktury `divided_file` jejichž členy budou korektně uvolněny.
 */
void divided_file_deinit(struct divided_file *divided_file);

/**
 * \brief Funkce uvolní dynamicky alokované pole řetězců. K uvolnění jednotlivých řetězců
 * v poli se použije funkce `line_dealloc`. Pole, na které `label` ukazuje bude nastaven na
 * hodnotu `NULL`, tzn. neplatný ukazatel.
 * \param label Ukazatel na pole řetězců, které bude uvolněno.
 * \param line_count Počet řádek, tedy délka pole.
 */
void label_dealloc(char ***label, const size_t line_count);

/**
 * \brief Funkce zkontroluje, zda jsou přítomny všechny povinné sekce.
 * \param file Ukazatel na instanci struktury `divided_file`, ve které bude přitomnost sekcí kontrolována.
 * \return int 1, pokud jsou všechny povinné sekce přítomny, jinak 0.
 */
int check_mandatory_labels(const struct divided_file *file);

/**
 * \brief Funkce zkontroluje, zda se na konci souboru, tedy za sekcí End, ještě něco nenachází.
 * \param file Soubor ke kontrole.
 * \return int 1, pokud se za sekcí End ještě něco nachází, jinak 0.
 */
int check_end_file(FILE *file);

/**
 * \brief Funkce dynamicky alokuje instanci struktury `processed_file`, kterou inicializuje pomocí funkce `processed_file_init`.
 * \param generals Ukazatel na dynamicky alokovanou strukturu `generals`, která se předá do zpracovaného souboru.
 * \param function Ukazatel na dynamicky alokovanou strukturu `function`, která se předá do zpracovaného souboru.
 * \param subject_to Ukazatel na dynamicky alokovanou strukturu `subject_to`, která se předá do zpracovaného souboru.
 * \param bounds Ukazatel na dynamicky alokovanou strukturu `bounds`, která se předá do zpracovaného souboru.
 * \return processed_file* Ukazatel na nově dynamicky alokovanou instanci struktury `processed_file`, nebo `NULL` při chybě.
 */
struct processed_file *processed_file_alloc(struct generals *generals, struct function *function, struct subject_to *subject_to, struct bounds *bounds);

/**
 * \brief Funkce pro inicializaci instance struktury `processed_file`.
 * \param processed_file Ukazatel na inicializovanou instanci struktury `processed_file`.
 * \param generals Ukazatel na dynamicky alokovanou strukturu `generals`, která se předá do zpracovaného souboru.
 * \param function Ukazatel na dynamicky alokovanou strukturu `function`, která se předá do zpracovaného souboru.
 * \param subject_to Ukazatel na dynamicky alokovanou strukturu `subject_to`, která se předá do zpracovaného souboru.
 * \param bounds Ukazatel na dynamicky alokovanou strukturu `bounds`, která se předá do zpracovaného souboru.
 * \return int 1, pokud inicializace processed_file proběhla v pořádku, jinak 0.
 */
int processed_file_init(struct processed_file *processed_file, struct generals *generals, struct function *function, struct subject_to *subject_to, struct bounds *bounds);

/**
 * \brief Funkce uvolní dynamicky alokovanou instanci struktury `processed_file`. K uvolnění vnitřních členů
 *        struktury používá funkce `processed_file_deinit`. Ukazatel, na který `processed_file` ukazuje bude nastaven na
 *        hodnotu `NULL`, tzn. neplatný ukazatel.
 * \param processed_file Ukazatel na ukazatel na instanci struktury `processed_file`, která bude uvolněna.
 */
void processed_file_dealloc(struct processed_file **processed_file);

/**
 * \brief Funkce provede korektní uvolnění členů instance struktury `processed_file`.
 * \param processed_file Ukazatel na instanci struktury `processed_file` jejichž členy budou korektně uvolněny.
 */
void processed_file_deinit(struct processed_file *processed_file);

/**
 * \brief Funkce zkontroluje, zda se v účelové funkci a v omezeních nenachází nepoužitá rozhodovací proměnná.
 * \param function Ukazatel na instanci struktury `function`, která se kontroluje.
 * \param function Ukazatel na instanci struktury `subject_to`, která se kontroluje.
 * \param generals Ukazatel na instanci struktury `generals` obsahující infomace o proměnných.
 */
void check_unused_variables(const struct function *function, const struct subject_to *subject_to, const struct generals *generals);

#endif