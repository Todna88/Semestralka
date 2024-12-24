#include "../label.h"

#ifndef OPERATORS_H
#define OPERATORS_H

#define PLUS '+'
#define MINUS '-'
#define MULTIPLY '*'

typedef double (*operation)(double, double);

struct operators{
    char operator;
    size_t priority;
    operation operate;
};

int get_priority(const char operator);

operation get_operation(const char operator);

double sum(double a, double b);

double sub(double a, double b);

double mul(double a, double b);

int array_mul(double *array, double multiplier, size_t arr_len);

int array_sum(double *array, double *array_2, size_t arr_len);

int is_operator(const char symbol);

#endif