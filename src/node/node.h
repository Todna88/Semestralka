/**
 * \file linked_list.h
 * \author Antonín Hobl (hobl@students.zcu.cz)
 * \brief Hlavičkový soubor s deklaracemi funkcí pracujících se spojovým prvek fronty.
 * \version 1.0
 * \date 2024-10-31
 */

#ifndef NODE_H
#define NODE_H

#include <stddef.h>

/** \brief Definice spojového seznamu. */

struct node{
    void *data;
    struct node *next;
};

struct node *node_alloc(const size_t data_size);

int node_init(struct node *s, const size_t data_size);

void node_deinit(struct node *n);

void node_dealloc(struct node **n);

int node_add_data(struct node *n, const void *data, const size_t data_size);

struct node *create_node(const size_t data_size, const void *data);

#endif