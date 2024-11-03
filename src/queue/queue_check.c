#include "queue.h"

#include"stdlib.h"
#include"stdio.h"

#define QUEUE_SIZE 10000

int main(){
    typedef int queue_type;
    queue_type i;
    struct queue *queue;
    queue_type result;

    queue = queue_alloc(sizeof(i));
    for (i = 0; i < QUEUE_SIZE; ++i){
        queue_enqueue(queue, &i);
    }

    for (i = 0; i < QUEUE_SIZE/2; ++i){
        queue_dequeue(queue, &result);
        printf("%d\n", result);
    }

    printf("\nNumber of items: %lu\n", queue->item_count);
    queue_dealloc(&queue);
    
    return EXIT_SUCCESS;
}