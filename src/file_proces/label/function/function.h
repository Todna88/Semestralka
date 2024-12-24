#include "operators.h"
#include "../generals/generals.h"
#include "../../../stack/stack.h"
#include "../../../queue/queue.h"
#include <ctype.h>

#ifndef FUNCTION_H
#define FUNCTION_H

#define DOT '.'

struct function{
    double *coefs;
};

struct queue_record{
    double *coef_values;
    size_t arr_len;
    double value;
    operation operator;
};

int parse_variable(struct queue *queue, struct stack *stack, const char *function, const struct generals *generals, int *sign);

int parse_operator(struct queue *queue, struct stack *stack, const char current_char, int *sign);

int parse_brackets(struct queue *queue, struct stack *stack, const char current_char);

int parse_number(struct stack *stack, struct queue *queue, const char *function, int *sign);

int check_variable(const char next_char);

int check_unary_operator(const char operator, int *sign);

int evaluate(struct queue_record *operand_1, struct queue_record *operand_2, struct queue *queue, operation operator);

int queue_record_init(struct queue_record *queue_record, double *coef_values, size_t arr_len, double value, operation operator);

struct function *process_function(char *function, const struct generals *generals);

int control_function(char *function);

struct function *function_alloc(size_t coef_count);

int function_init(struct function *function, size_t coef_count);

void function_dealloc(struct function **function);

void function_deinit(struct function *function);

int is_left_bracket(const char symbol);

int is_right_bracket(const char symbol);

int is_equal_bracket(const char left_bracket, const char right_bracket);

#endif