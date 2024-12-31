#include "operators.h"
#include "../generals/generals.h"
#include "../../../stack/stack.h"
#include <ctype.h>

#ifndef FUNCTION_H
#define FUNCTION_H

#define DOT '.'

#define MIN_TYPE "Minimalize"

#define MAX_TYPE "Maximalize"

struct function{
    double *coefs;
};

struct output_record{
    double *coef_values;
    size_t arr_len;
    double value;
};

int parse_variable(struct stack *output_stack, struct stack *input_stack, const char *function, const struct generals *generals);

int parse_operator(struct stack *output_stack, struct stack *input_stack, const char current_char);

int parse_brackets(struct stack *output_stack, struct stack *input_stack, const char current_char);

int parse_number(struct stack *input_stack, struct stack *output_stack, const char *function);

int check_variable(const char next_char);

int check_unary_operator(const char operator, struct stack *output_stack, struct stack *input_stack);

int evaluate(struct output_record *operand_1, struct output_record *operand_2, struct stack *output_stack, operation operator);

int output_record_init(struct output_record *output_record, double *coef_values, size_t arr_len, double value);

struct function *process_function(char *function, char *function_type, const struct generals *generals);

int control_function(char *function);

struct function *function_alloc(size_t coef_count);

int function_init(struct function *function, size_t coef_count);

void function_dealloc(struct function **function);

void function_deinit(struct function *function);

void record_arrays_dealloc(struct stack *output_stack);

int is_left_bracket(const char symbol);

int is_right_bracket(const char symbol);

int is_equal_bracket(const char left_bracket, const char right_bracket);

void delete_spaces(char * line);

void func_print_coefs(struct function *function, struct generals *generals);

int check_stacks(double *coef_array, struct stack *input_stack, struct stack *output_stack, size_t var_count);

int parse_artithmetic_expression(char *expression, double *coef_array, const struct generals *generals);

void dealloc_record(struct output_record *record);

int check_type_of_task(struct function *function, char *function_type, const size_t var_count);

#endif