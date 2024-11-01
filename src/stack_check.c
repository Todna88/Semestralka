#include "stack/stack.h"

#include"stdlib.h"
#include"stdio.h"

#define STACK_SIZE 10000

int main(){
    int i;
    struct stack *stack;
    int result;

    stack = stack_alloc(sizeof(i));
    for (i = 0; i < STACK_SIZE; ++i){
        stack_push(stack, &i);
    }

    for (i = 0; i < STACK_SIZE/2; ++i){
        stack_pop(stack, &result);
        printf("%d\n", result);
    }

    stack_dealloc(&stack);
    
    return EXIT_SUCCESS;
}