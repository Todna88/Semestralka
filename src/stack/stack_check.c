#include "stack.h"

#include"stdlib.h"
#include"stdio.h"

#define STACK_SIZE 10000

int main(){
    typedef int stack_type;
    stack_type i;
    struct stack *stack;
    stack_type result;

    stack = stack_alloc(sizeof(i));
    for (i = 0; i < STACK_SIZE; ++i){
        stack_push(stack, &i);
    }

    for (i = 0; i < STACK_SIZE/2; ++i){
        stack_pop(stack, &result);
        printf("%d\n", result);
    }

    printf("\nNumber of items: %lu\n", stack->item_count);
    stack_dealloc(&stack);
    
    return EXIT_SUCCESS;
}