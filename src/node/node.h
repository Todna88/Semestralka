/**
 * \file node.h
 * \author Antonín Hobl (hobl@students.zcu.cz)
 * \brief Hlavičkový soubor s deklaracemi funkcí pracujících se spojovým uzlem.
 * \version 1.0
 * \date 2024-10-31
 */

#ifndef NODE_H
#define NODE_H

#include <stddef.h>

/** \brief Definice spojového uzlu s možností vložení libovolně velkého prvku. */

struct node{
    void *data;
    struct node *next;
    size_t data_size;
};

#define DEFAULT_NODE {NULL, NULL, 0}

/**
 * \brief Funkce dynamicky alokuje instanci struktury `node`, kterou inicializuje pomocí funkce `node_init`.
 * \param data_size Velikost jednoho prvku spojového uzlu.
 * \return node* Ukazatel na nově dynamicky alokovanou instanci struktury `node`, nebo `NULL` při chybě.
 */
struct node *node_alloc(const size_t data_size);

/**
 * \brief Funkce pro inicializaci instance struktury `node`.
 * \param n Ukazatel na inicializovanou instanci struktury `node`.
 * \param data_size Velikost jednoho prvku spojového uzlu.
 * \return int 1, pokud inicializace spojového uzlu proběhla v pořádku, jinak 0.
 */
int node_init(struct node *n, const size_t data_size);

/**
 * \brief Funkce provede korektní uvolnění členů instance struktury `node`.
 * \param n Ukazatel na instanci struktury `node` jejichž členy budou korektně uvolněny.
 */
void node_deinit(struct node *n);

/**
 * \brief Funkce uvolní dynamicky alokovanou instanci struktury `node`. K uvolnění vnitřních členů
 *        struktury používá funkce `node_deinit`. Ukazatel, na který `n` ukazuje bude nastaven na
 *        hodnotu `NULL`, tzn. neplatný ukazatel.
 * \param n Ukazatel na ukazatel na instanci struktury `node`, která bude uvolněna.
 */
void node_dealloc(struct node **n);

/**
 * \brief Funkce vloží nový prvek na adrese `data` do spojového uzlu, který je dán ukazatelem `n` na instanci struktury `node`.
 * \param n Ukazatel na instanci struktury `node`, do které boudou vložena nová data.
 * \param data Ukazatel na vkládaná data.
 * \return int 1 pokud vložení (kopírování) prvku do zásobníku dopadlo dobře, jinak 0.
 */
int node_add_data(struct node *n, const void *data);

/**
 * \brief Funkce dynamicky alokuje nový spojový uzel `node` pomocí funkce `node_alloc` a následně ho naplní daty pomocí funkce `node_add_data`.
 * \param data_size Velikost jednoho prvku spojového uzlu.
 * \param data Ukazatel na vkládaný prvek.
 * \return node* Ukazatel na nově dynamicky alokovanou instanci struktury `node` naplněnou daty, nebo `NULL` při chybě.
 */
struct node *create_node(const size_t data_size, const void *data);

#endif