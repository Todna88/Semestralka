#include "node.h"
#include "../errors.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct node *node_alloc(const size_t data_size){
    struct node *new_node;
    
    new_node = malloc(sizeof(*new_node));

    if (!new_node){
        error = MEMORY_ERR;
        return NULL;
    }

    if(!node_init(new_node, data_size)){
        free(new_node);
        return NULL;
    }

    return(new_node);
}

int node_init(struct node *n, const size_t data_size){
    if (!n || data_size == 0){
        error = POINTER_ERR;
        return 0;
    }

    n->data = malloc(data_size);

    if (!n->data){
        error = MEMORY_ERR;
        return 0;
    }

    n->data_size = data_size;
    n->next = NULL;

    return 1;   
}

void node_deinit(struct node *n){
    if(!n){
        return;
    }

    n->data_size = 0;
    n->next = NULL;

    if(n->data){
        free(n->data);
        n->data = NULL;
    }

    return;
}

void node_dealloc(struct node **n){
    if (!n || !(*n)){
        return;
    }

    node_deinit(*n);

    free(*n);
    *n = NULL;
}

int node_add_data(struct node *n, const void *data){
    if(!n || !data){
        error = POINTER_ERR;
        return 0;
    }

    memcpy(n->data, data, n->data_size);

    return 1;
}

struct node *create_node(const size_t data_size, const void *data){
    struct node *new_node;
    if (data_size == 0 || (!data)){
        error = POINTER_ERR;
        return NULL;
    }

    new_node = node_alloc(data_size);

    if (!new_node){
        return NULL;
    }

    if(!node_add_data(new_node, data)){
        return NULL;
    }

    return new_node;  
}