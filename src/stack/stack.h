/**
 * \file stack.h
 * \author Antonín Hobl (hobl@students.zcu.cz)
 * \brief Hlavičkový soubor s deklaracemi funkcí pracujících se zásobníkovou strukturou.
 * \version 1.0
 * \date 2024-10-31
 */

#ifndef STACK_H
#define STACK_H

#include <stddef.h>

/** \brief Definice zásobníkové struktury s možností vložení libovolně velkého prvku. */

struct stack{
    struct node *top;
    size_t item_count;
    size_t data_size;
};

#define DEFAULT_STACK {NULL, 0, 0}

/**
 * \brief Funkce dynamicky alokuje instanci struktury `stack`, kterou inicializuje pomocí funkce `stack_init`.
 * \param data_size Velikost jednoho prvku zásobníku.
 * \return stack* Ukazatel na nově dynamicky alokovanou instanci struktury `stack`, nebo `NULL` při chybě.
 */
struct stack *stack_alloc(const size_t data_size);

/**
 * \brief Funkce pro inicializaci instance struktury `stack`.
 * \param s Ukazatel na inicializovanou instanci struktury `stack`.
 * \param data_size Velikost jednoho prvku zásobníku.
 * \return int 1, pokud inicializace zásobníku proběhla v pořádku, jinak 0.
 */
int stack_init(struct stack *s, const size_t data_size);

/**
 * \brief Funkce provede korektní uvolnění členů instance struktury `stack`.
 * \param s Ukazatel na instanci struktury `stack` jejichž členy budou korektně uvolněny.
 */
void stack_deinit(struct stack *s);

/**
 * \brief Funkce uvolní dynamicky alokovanou instanci struktury `stack`. K uvolnění vnitřních členů
 *        struktury používá funkce `stack_deinit`. Ukazatel, na který `s` ukazuje bude nastaven na
 *        hodnotu `NULL`, tzn. neplatný ukazatel.
 * \param s Ukazatel na ukazatel na instanci struktury `stack`, která bude uvolněna.
 */
void stack_dealloc(struct stack **s);

/**
 * \brief Funkce zkontroluje, zda je zásobník prázdný.
 * \param s Ukazatel na instanci struktury `stack`, u které se ptáme na naplněnost.
 * \return int 1 pokud je prázdný, jinak 0.
 */
int check_empty(const struct stack *s);

/**
 * \brief Funkce vloží nový prvek na adrese `data` do zásobníku, který je dán ukazatelem `s` na instanci struktury `stack`.
 * \param s Ukazatel na instanci struktury `stack`, do které bude vložen nový záznam.
 * \param data Ukazatel na vkládaný prvek.
 * \return int 1 pokud vložení (kopírování) prvku do zásobníku dopadlo dobře, jinak 0.
 */
int stack_push(struct stack *s, const void *data);

/**
 * \brief Funkce odebere prvek z vrcholu zásobníku daného ukazatelem `s`. Odebraný prvek bude zkopírován do oblasti paměti,
 *        dané ukazatelem `data` (pokud je předaný ukazatel nenulový).
 * \param s Ukazatel na instanci struktury `stack`, ze které bude prvek odebrán.
 * \param data Ukazatel na oblast paměti, kam bude odebraný prvek zkopírován.
 * \return int 1, pokud vše dopadlo dobře, jinak 0.
 */
int stack_pop(struct stack *s, void *data);

/**
 * \brief Funkce zkopíruje prvek z vrcholu zásobníku do oblasti paměti dané ukazatel `data`.
 * \param s Ukazatel na instanci struktury `stack`, jejíž poslední přidaný prvek budeme kopírovat.
 * \param data Ukazatel na oblast paměti, kam se přečtený prvek zkopíruje.
 * \return int 1, pokud kopírování dopadlo dobře, jinak 0.
 */
int stack_head(const struct stack *s, void *data);

#endif