#include "stack.h"
#include "../errors.h"
#include "../node/node.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct stack *stack_alloc(const size_t data_size){
    struct stack *new_stack;

    new_stack = malloc(sizeof(*new_stack));

    if (!new_stack){
        error = MEMORY_ERR;
        return NULL;
    }
    
    if(!stack_init(new_stack, data_size)){
        free(new_stack);
        return NULL;
    }

    return(new_stack);
}

int stack_init(struct stack *s, const size_t data_size){
    if(!s || data_size == 0){
        error = POINTER_ERR;
        return 0;
    }

    s->top = NULL;
    s->item_count = 0;
    s->data_size = data_size;
    
    return 1;  
}

void stack_deinit(struct stack *s){
    struct node *tmp;
    if (!s){
        return;
    }

    s->item_count = 0;
    s->data_size = 0;

    while (s->top != NULL){
        tmp = s->top->next;
        node_dealloc(&(s->top));
        s->top = tmp;
    }

    s->top = NULL;

    return;
}

void stack_dealloc(struct stack **s){
    if (!s || !(*s)){
        return;
    }
    
    stack_deinit(*s);

    free(*s);
    *s = NULL;

    return;
}

int check_empty(const struct stack *s){
    if (s->item_count == 0){
        return 1;
    }
    
    return 0;
    
}

int stack_push(struct stack *s, const void *data){
    struct node *new_node;
    if(!s || !data){
        error = POINTER_ERR;
        return 0;
    }

    new_node = create_node(s->data_size, data);

    if (!new_node){
        return 0;
    }
    
    new_node->next = s->top;
    s->top = new_node;
    s->item_count++;
    return 1;
}

int stack_pop(struct stack *s, void *data){
    struct node *tmp;
    if (!stack_head(s, data)){
        return 0;
    }

    tmp = s->top ->next;
    node_dealloc(&(s->top));
    s->top = tmp;
    s->item_count--;
    return 1;
}

int stack_head(const struct stack *s, void *data){
    if (!s || !data || s->item_count == 0){
        error = POINTER_ERR;
        return 0;
    }

    memcpy(data, s->top->data, s->data_size);

    return 1;
}