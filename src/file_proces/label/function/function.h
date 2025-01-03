/**
 * \file function.h
 * \author Antonín Hobl (hobl@students.zcu.cz)
 * \brief Hlavičkový soubor s deklaracemi funkcí nutných pro zpracování účelové funkce ze vstupním souboru.
 * \version 1.0
 * \date 2025-01-02
 */

#ifndef FUNCTION_H
#define FUNCTION_H

#include "operators.h"
#include "../generals/generals.h"
#include "../../../stack/stack.h"
#include <ctype.h>

#define DOT '.'
#define DOT_WAS 1

#define MIN_TYPE "Minimalize"
#define MAX_TYPE "Maximalize"

#define LEFT_NORMAL_BRACKET '('
#define LEFT_COMPOUND_BRACKET '{'
#define LEFT_SQUARE_BRACKET '['

#define RIGHT_NORMAL_BRACKET ')'
#define RIGHT_COMPOUND_BRACKET '}'
#define RIGHT_SQUARE_BRACKET ']'

#define NEGATIVE_MILTIPLIER -1.0

#define ERROR_RESULT -1

#define SKIP_ONE_CHAR 2
#define NEXT_CHAR 1

#define FIRST_ITERATION 0
#define SECOND_ITERATION 1
#define THIRD_ITERATION 2

#define DEFAULT_VAR_COEF 1.0

#define RECORD_VAR_VALUE 0
#define RECORD_NUM_ARR_LEN 0

/** \brief Definice struktury, ve které budou uloženy všechny potřebná data z účelové funkce*/

struct function{
    double *coefs;
};

/** \brief Definice struktury reprezentující záznam ve výstupním zásobníku*/

struct output_record{
    double *coef_values;
    size_t arr_len;
    double value;
};

/**
 * \brief Funkce zpracuje řádku s účelovou funkcí `function` pomocí funkce `parse_artithmetic_expression` a vrátí dynamicky alokovanou 
 * strukturu `function` alokovanou pomocí funkce `function_alloc`.
 * \param function Řetězec obsahující účelovou funkci.
 * \param function_type Řetězec obsahující informaci, zda se má účelová funkce maximalizovat, nebo minimalizovat.
 * \param generals Ukazatel na instanci struktury `generals`, potřebnout kvůli kvůli informacím o proměnných. 
 * \return function* Ukazatel na dynamicky alokovanou instanci struktury `function`, obsahující zpracovaná data, nebo `NULL` při chybě.
 */
struct function *process_function(char *function, const char *function_type, const struct generals *generals);

/**
 * \brief Funkce zkontroluje zda se předaný znak `next_char` muže násobit bez operátoru *.
 * \param next Znak, který je kontrolován na násobení bez operátoru.
 * \return int 1, pokud lze násobit bez operátoru, 0 pokud nelze.
 */
int check_multiply(const char next_char);

/**
 * \brief Funkce vyparsuje název proměnné z řetězce `function` a vloží její záznam do výstupního zásobníku `output_stack`, následně pomocí
 * funkce `check multiply` zkontroluje, zda nelze následující znak násobit bez použití operátoru *, pokud ano, pak vloží daný
 * operátor do vstupního zásobníku pomocí funkce `parse_operator`.
 * \param output_stack Ukazatel na instanci struktury `stack`, do které bude uložen záznam vyparsované proměnné.
 * \param input_stack Ukazatel na instanci struktury `stack`, do kterého bude uložen operátor *, pokud lze následující znak násobit bez jeho použití.
 * \param function Řetězec začínající znakem, kterým muže začínat název proměnné
 * \param generals Ukazatel na instanci struktury `generals`, potřebnout kvůli informacím o proměnných.
 * \return int počet znaků, o který se funkce posunula v řetězci `function`, -1 při chybě.
 */
int parse_variable(struct stack *output_stack, struct stack *input_stack, const char *function, const struct generals *generals);

/**
 * \brief Funkce vyparsuje typ operátoru ze znaku `current_char` a provede operace ekvivalentní algoritmu Shunting Yard při zpracování operátoru.
 * \param output_stack Ukazatel na instanci struktury `stack`, kam se uloží záznam s výsledkem operace operátoru na vrcholu vstupního zásobníku 
 * `input_stack` pokud má vyšší nebo stejnou prioritou jako má vkládaný operátor (viz. Shunting Yard).
 * \param input_stack Ukazatel na instanci struktury `stack`, kam se vloží vyparsovaný operátor.
 * \param current_char Znak obsahující některý z daných aritmetických operátorů.
 * \return int 1 pokud vše proběhlo v pořádku, jinak 0.
 */
int parse_operator(struct stack *output_stack, struct stack *input_stack, const char current_char);

/**
 * \brief Funkce zkontroluje zda je předný znak `current_char` unární operátor, pokud je kladný nestane se nic, pokud je záporný,
 * tak se do výstupního zásobníku `output_stack` vloží záznam reprezentující hodnotu -1 a do vstupního zásobníku `input_stack`se vloží operátor
 * násobení. Pokud je operátor násobení, pak je to syntaktická chyba, pokus to není operátor vůbec nestane se nic.
 * \param output_stack Ukazatel na instanci struktury `stack`, kam se vloží záznam reprezentující hodnotu -1, pokud je operátor -.
 * \param input_stack Ukazatel na instanci struktury `stack`, kam se vloží operátor násobení, pokud je operátor -.
 * \param current_char Znak na kontrolu unárního operátoru.
 * \return int 1 pokud je to unární operátor, 0 pokud není, nebo -1 při chybě.
 */
int check_unary_operator(const char operator, struct stack *output_stack, struct stack *input_stack);

/**
 * \brief Funkce vypočítá výraz uvnitř závorek ekvivalentně k algoritmu Shunting Yard. S jednotlivými operátory rovnou provede jejich operace pomocí funkce `evaluate`.
 * Poté co funkce narazí na levou závorku, zkontroluje zda je ekvivalentní k vstupní pravé závorce pomocí funkce `is_equal_bracket`.
 * \param output_stack Ukazatel na instanci struktury `stack`, kam se úkládají výsledky opercí.
 * \param input_stack Ukazatel na instanci struktury `stack`, ze které se postupně berou operátory, než se narazí na levou závorku.
 * \param current_char Znak obsahující některou z pravých závorek.
 * \return int 1 pokud vše proběhlo v pořádku, jinak 0.
 */
int parse_brackets(struct stack *output_stack, struct stack *input_stack, const char current_char);

/**
 * \brief Funkce provede operaci danou funkcí `operator`, mezi operadny `operand_1` a `operand_1` a uloží výsledek do výstpního zásobníku `output_stack`.
 * Provádění operací mezi operandy závisí na tom, jestli jsou to čísla nebo proměnné.
 * \param operand_1 Ukazatel na instanci struktury `output_record` reprezentující první operand.
 * \param operand_2 Ukazatel na instanci struktury `output_record` reprezentující druhý operand.
 * \param output_stack Ukazatel na instanci struktury `stack`, kam se uloží záznam s výsledkem.
 * \param operator Ukazatel na funkci, která reprezentuje operaci mezi operandy.
 * \return int 1 pokud vše proběhlo v pořádku, jinak 0.
 */
int evaluate(struct output_record *operand_1, struct output_record *operand_2, struct stack *output_stack, const operation operator);

/**
 * \brief Funkce vyparsuje číslo z řetězce `function` a vloží jejho záznam do výstupního zásobníku `output_stack`, následně pomocí
 * funkce `check multiply` zkontroluje, zda nelze následující znak násobit bez použití operátoru *, pokud ano, pak vloží daný
 * operátor do vstupního zásobníku `input_stack` pomocí funkce `parse_operator`.
 * \param output_stack Ukazatel na instanci struktury `stack`, do které bude uložen záznam vyparsovaného čísla.
 * \param input_stack Ukazatel na instanci struktury `stack`, do kterého bude uložen operátor *, pokud lze následující znak násobit bez jeho použití.
 * \param function Řetězec začínající znakem, kterým muže začínat číslo.
 * \return int počet znaků, o který se funkce posunula v řetězci `function`, -1 při chybě.
 */
int parse_number(struct stack *input_stack, struct stack *output_stack, const char *function);

/**
 * \brief Funkce pro inicializaci instance struktury `output_record`.
 * \param output_record Ukazatel na inicializovanou instanci struktury `output_record`.
 * \param coef_values Pole koeficientů, které se zkopíruje do vnitřního pole struktury `output_record`
 * \param arr_len Délka pole `coef_values`.
 * \param value Hodnota, která se zkopíruje do proměnné uvnitř struktury `output_record`.
 * \return \return int 1, pokud inicializace zásobníku proběhla v pořádku, jinak 0.
 */
int output_record_init(struct output_record *output_record, double *coef_values, size_t arr_len, double value);

/**
 * \brief Funkce zkontroluje, zda předaný řetězec `function`, ve kterém je uložena účelová funkce, neobsahuje zakázané znaky.
 * \param function Řetězec s účelovou funkcí, který se kontroluje.
 * \return int 0 pokud obsahuje zakázaný znak, jinak 1.
 */
int control_function(const char *function);

/**
 * \brief Funkce dynamicky alokuje instanci struktury `function`, kterou inicializuje pomocí funkce `function_init`.
 * \param var_count Počet koeficientů proměnných.
 * \return function* Ukazatel na nově dynamicky alokovanou instanci struktury `function`, nebo `NULL` při chybě.
 */
struct function *function_alloc(const size_t coef_count);

/**
 * \brief Funkce pro inicializaci instance struktury `function`.
 * \param bounds Ukazatel na inicializovanou instanci struktury `function`.
 * \param var_count Počet koeficientů proměnných.
 * \return int 1, pokud inicializace function proběhla v pořádku, jinak 0.
 */
int function_init(struct function *function, const size_t coef_count);

/**
 * \brief Funkce uvolní dynamicky alokovanou instanci struktury `function`. K uvolnění vnitřních členů
 *        struktury používá funkce `function_deinit`. Ukazatel, na který `function` ukazuje bude nastaven na
 *        hodnotu `NULL`, tzn. neplatný ukazatel.
 * \param function Ukazatel na ukazatel na instanci struktury `function`, která bude uvolněna.
 */
void function_dealloc(struct function **function);

/**
 * \brief Funkce provede korektní uvolnění členů instance struktury `function`.
 * \param function Ukazatel na instanci struktury `function` jejichž členy budou korektně uvolněny.
 */
void function_deinit(struct function *function);

/**
 * \brief Funkce zkontroluje, zda je předaný znak `symbol` jedna z levých závorek.
 * \param function Znak, u kterého se určuje, zda je jedna z levých závorek.
 * \return int 1 pokud je, 0 pokud není.
 */
int is_left_bracket(const char symbol);

/**
 * \brief Funkce zkontroluje, zda je předaný znak `symbol` jedna z pravých závorek.
 * \param function Znak, u kterého se určuje, zda je jedna z pravých závorek.
 * \return int 1 pokud je, 0 pokud není.
 */
int is_right_bracket(const char symbol);

/**
 * \brief Funkce zkontroluje, zda je levá závorka `left_bracket` stejného typu jako pravá závorka `right_bracket`.
 * \param left_bracket Znak levé kontrolované závorky.
 * \param right_bracket Znak pravé kontrolované závorky.
 * \return 1 pokud jsou stejného typu, jinak 0.
 */
int is_equal_bracket(const char left_bracket, const char right_bracket);

/**
 * \brief Funkce vymaže z předaného řetězce `line` všechny bílé znaky.
 * \param line Řetězec, ve kterém jsou vymazávány bílé znaky.
 */
void delete_spaces(char *line);


/**
 * \brief Funkce zkontroluje, zda jsou vstupní zásobník `input_stack` a výstupní zásobník `output_stack` prázdné, pokud nejsou, tak
 * všehny hodnoty vyberou a vyhodnotí. Nakonec se výsledná hodnota vnitřního pole struktury `output_record` zkopíruje do pole `coef_array`.
 * \param coef_array Pole koeficientů, do kterého se zkopíruje vnitřní pole výsledné hodnoty ve výstupním zásobníku `output_stack`.
 * \param input_stack Vstupní zásobník, u kterého se kontroluje zda je prázdný.
 * \param output_stack Výstupník zásobník, ze kterého bude nakonec vybrán výslední záznam s obsahem koeficientů u proměnných.
 * \param var_count Počet všech proměnných.
 * \return 1 pokud vše dopadlo dobře, jinak 0.
 */
int check_stacks(double *coef_array, struct stack *input_stack, struct stack *output_stack, const size_t var_count);

/**
 * \brief Funkce vyparsuje pomocí dalších funkcí koeficienty u proměnných z aritmetického výrazu `expression`. Koeficienty budou nakonec uloženy v poli `coef_array`.
 * \param expression Řetězec obsahující daný aritmetický výraz.
 * \param coef_array Výsledné pole, které bude nakonci obsahovat koeficienty proměnných.
 * \param generals Ukazatel na instanci struktury `generals`, obsahující počet proměnných a pole jejich názvů.
 * \return 1 pokud vše dopadlo dobře, jinak 0.
 */
int parse_artithmetic_expression(char *expression, double *coef_array, const struct generals *generals);


/**
 * \brief Funkce postupně vybere všechny záznamy ze zásobníku `output_stack` a provede dealokaci jejich vnitřních členů pomocí funkce `dealloc_record`.
 * \param output_stack Výstupní zásobník, který bude vyprázdněn a členy jeho záznamů budou uvolněny.
 */
void record_arrays_dealloc(struct stack *output_stack);

/**
 * \brief Funkce uvolní dynamicky alokované vnitřní pole instance struktury `output_record` a jeho ukazatel nasraví na hodnotu `NULL`, tzn. neplatný ukazatel.
 * \param record Záznam, jehož vnitřní pole bude uvolněno.
 */
void dealloc_record(struct output_record *record);

/**
 * \brief Funkce nastaví koeficienty u vnitřního pole struktury `function`, podle toho zda se funkce minimalizuje nebo maximalizuje. Pokud
 * se maximalizuje, pak se nic nestane, pokud se minimalizuje, pak se u všech koeficientů promenných obrátí znaménka.
 * \param function Ukazatel na dynamicky alokovanou instanci struktury `function`, ve které se v případě minimalizace obrátí znaménka ve vnitřním poli.
 * \param function_type Řetězec který obsahuje "Minimalize" při minimalizaci funkce, nebo "Maximalize" při maximalizaci funkce.
 * \param var_count Počet všech proměnných.
 * \return int 1 pokud vše dopadlo dobře, jinak 0.
 */
int check_type_of_task(struct function *function, const char *function_type, const size_t var_count);

#endif