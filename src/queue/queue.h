/**
 * \file queue.h
 * \author Antonín Hobl (hobl@students.zcu.cz)
 * \brief Hlavičkový soubor s deklaracemi funkcí pracujících se strukturou fronta.
 * \version 1.0
 * \date 2024-11-03
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>

/** \brief Definice struktury fronta s možností vložení libovolně velkého prvku. */

struct queue{
    struct node *first;
    struct node *last;
    size_t item_count;
    size_t data_size;
};

#define DEFAULT_STACK {NULL, NULL, 0, 0}

/**
 * \brief Funkce dynamicky alokuje instanci struktury `queue`, kterou inicializuje pomocí funkce `queue_init`.
 * \param data_size Velikost jednoho prvku fronty.
 * \return queue* Ukazatel na nově dynamicky alokovanou instanci struktury `queue`, nebo `NULL` při chybě.
 */
struct queue *queue_alloc(const size_t data_size);

/**
 * \brief Funkce pro inicializaci instance struktury `queue`.
 * \param q Ukazatel na inicializovanou instanci struktury `queue`.
 * \param data_size Velikost jednoho prvku fronty.
 * \return int 1, pokud inicializace fronty proběhla v pořádku, jinak 0.
 */
int queue_init(struct queue *q, const size_t data_size);

/**
 * \brief Funkce provede korektní uvolnění členů instance struktury `queue`.
 * \param q Ukazatel na instanci struktury `queue` jejichž členy budou korektně uvolněny.
 */
void queue_deinit(struct queue *q);

/**
 * \brief Funkce uvolní dynamicky alokovanou instanci struktury `queue`. K uvolnění vnitřních členů
 *        struktury používá funkce `queue_deinit`. Ukazatel, na který `q` ukazuje bude nastaven na
 *        hodnotu `NULL`, tzn. neplatný ukazatel.
 * \param q Ukazatel na ukazatel na instanci struktury `queue`, která bude uvolněna.
 */
void queue_dealloc(struct queue **q);

/**
 * \brief Funkce zkontroluje, zda je fronta prázdná.
 * \param q Ukazatel na instanci struktury `queue`, u které se ptáme na naplněnost.
 * \return int 1 pokud je prázdná, jinak 0.
 */
int is_empty(const struct queue *q);

/**
 * \brief Funkce vloží nový prvek na adrese `data` do fronty, která je dána ukazatelem `q` na instanci struktury `queue`.
 * \param q Ukazatel na instanci struktury `queue`, do které bude vložen nový záznam.
 * \param data Ukazatel na vkládaný prvek.
 * \return int 1 pokud vložení (kopírování) prvku do fronty dopadlo dobře, jinak 0.
 */
int queue_enqueue(struct queue *q, const void *data);

/**
 * \brief Funkce odebere prvek ze začátku fronty dané ukazatelem `q`. Odebraný prvek bude zkopírován do oblasti paměti,
 *        dané ukazatelem `data` (pokud je předaný ukazatel nenulový).
 * \param q Ukazatel na instanci struktury `queue`, ze které bude prvek odebrán.
 * \param data Ukazatel na oblast paměti, kam bude odebraný prvek zkopírován.
 * \return int 1, pokud vše dopadlo dobře, jinak 0.
 */
int queue_dequeue(struct queue *q, void *data);

/**
 * \brief Funkce zkopíruje prvek ze začátku fronty do oblasti paměti dané ukazatel `data`.
 * \param q Ukazatel na instanci struktury `queue`, jejíž prvně přidaný prvek budeme kopírovat.
 * \param data Ukazatel na oblast paměti, kam se přečtený prvek zkopíruje.
 * \return int 1, pokud kopírování dopadlo dobře, jinak 0.
 */
int queue_peek(const struct queue *q, void *data);

#endif