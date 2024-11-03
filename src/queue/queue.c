#include "queue.h"
#include "../node/node.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct queue *queue_alloc(const size_t data_size){
    struct queue *new_queue;

    new_queue = malloc(sizeof(*new_queue));

    if (!new_queue){
        return NULL;
    }
    
    if(!queue_init(new_queue, data_size)){
        free(new_queue);
        return NULL;
    }

    return(new_queue);
}

int queue_init(struct queue *q, const size_t data_size){
    if(!q || data_size == 0){
        return 0;
    }

    q->first = q->last = NULL;
    q->item_count = 0;
    q->data_size = data_size;
    
    return 1;  
}

void queue_deinit(struct queue *q){
    struct node *tmp;
    if (!q){
        return;
    }

    q->item_count = 0;
    q->data_size = 0;

    while (q->first != NULL){
        tmp = q->first->next;
        node_dealloc(&(q->first));
        q->first = tmp;
    }

    q->first = NULL;
    q->last = NULL;

    return;
}

void queue_dealloc(struct queue **q){
    if (!q || !(*q)){
        return;
    }
    
    queue_deinit(*q);

    free(*q);
    *q = NULL;

    return;
}

int is_empty(const struct queue *q){
    if (q->item_count == 0){
        return 1;
    }
    
    return 0;
    
}

int queue_enqueue(struct queue *q, const void *data){
    struct node *new_node;
    if(!q || !data){
        return 0;
    }

    new_node = create_node(q->data_size, data);

    if (!new_node){
        return 0;
    }
    
    if (q->item_count == 0){
        q->first = q->last = new_node;
        q->item_count++;
        return 1;
    }

    q->last->next = new_node;
    q->last = new_node;
    q->item_count++;
    return 1;
}

int queue_dequeue(struct queue *q, void *data){
    struct node *tmp;
    if (!queue_peek(q, data)){
        return 0;
    }

    tmp = q->first->next;
    node_dealloc(&(q->first));
    q->first = tmp;
    q->item_count--;
    return 1;
    
}

int queue_peek(const struct queue *q, void *data){
    if (!q || !data || q->item_count == 0){
        return 0;
    }

    memcpy(data, q->first->data, q->data_size);

    return 1;
}