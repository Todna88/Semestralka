#include "operators.h"

const struct operators OPERATORS[] = {
    { '+', 1, sum },
    { '-', 1, sub },
    { '*', 2, mul },
};

const size_t OPERATORS_COUNT = sizeof(OPERATORS) / sizeof(*OPERATORS);

int get_priority(const char operator){
    size_t i;

    for (i = 0; i < OPERATORS_COUNT; ++i) {
        if (OPERATORS[i].operator == operator) {
            return OPERATORS[i].priority;
        }
    }

    return -1;   
}

operation get_operation(const char operator){
    size_t i;

    for (i = 0; i < OPERATORS_COUNT; ++i) {
        if (OPERATORS[i].operator == operator) {
            return OPERATORS[i].operate;
        }
    }

    return NULL;
}

double sum(const double a, const double b){
    return a + b;
}

double sub(const double a, const double b){
    return a - b;
}

double mul(const double a, const double b){
    return a * b;
}

int array_mul(double *array, const double multiplier, const size_t arr_len){
    size_t i;
    if (!array || arr_len == 0){
        error = POINTER_ERR;
        return 0;
    }

    for (i = 0; i < arr_len; ++i){
        array[i] = array[i] * multiplier;
    }

    return 1;
    
}

int array_sum(double *array, const double *array_2, const size_t arr_len){
    size_t i;
    if (!array || !array_2 || arr_len == 0){
        error = POINTER_ERR;
        return 0;
    }

    for (i = 0; i < arr_len; ++i){
        array[i] = array[i] + array_2[i];
    }

    return 1;
}

int is_operator(const char symbol){
    if (symbol == PLUS || symbol == MINUS || symbol == MULTIPLY){
        return 1;
    }
    
    return 0;
}