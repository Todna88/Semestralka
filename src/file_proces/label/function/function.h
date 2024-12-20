#include "../label.h"

#ifndef FUNCTION_H
#define FUNCTION_H

struct function{
    double *coefs;
    char **vars;
    size_t coef_count;
};

struct function *process_function(char *function);

int control_function(char *function);

struct function *function_alloc();

int function_init(struct function *function);

void function_dealloc(struct function **function);

void function_deinit(struct function *function);

int is_left_bracket(const char symbol);

int is_right_bracket(const char symbol);

int is_equal_bracket(const char left_bracket, const char right_bracket);

int is_operator(char symbol);

double sum(double a, double b);

double sub(double a, double b);

double mul(double a, double b);




#endif