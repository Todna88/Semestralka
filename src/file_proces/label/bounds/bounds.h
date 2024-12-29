#include "../function/function.h"

#ifndef BOUNDS_H
#define BOUNDS_H

#define BOUNDS_ARRAY_COUNT 3
#define VALUES_ARRAY_COUNT 2
#define SPACE " "
#define EQUAL '='
#define NAN 0.0 / 0.0
#define MINUSINF -1.0 / 0.0
#define PLUSINF 1.0 / 0.0

union Infinity{
    double d;
    long l;
};

struct bounds{
    char **bounds;
    double **values;
};

struct bounds *process_bounds(char **bounds, size_t line_count, const struct generals *generals);

int parse_bound(struct bounds *bounds, char *line, const struct generals* generals);

void cpy_values(struct bounds *bounds, double *values, int variable_id);

int check_syntax(int variable_id, double *values, char *constr, size_t values_len);

int parse_var(int *variable_id, size_t *equal_flag, char *constr, char *token, int i, const struct generals *generals, double *values);

int parse_oper(char last_char, char *constr, int i, size_t *equal_flag, int variable_id);

int parse_num(int variable_id, size_t *equal_flag, double *values, char *constr, char *token, int i);

int check_operator(char operator);

void switch_operator(char *operator);

int parse_free(struct bounds *bounds, char *line, const struct generals* generals);

int set_free_var(struct bounds *bounds, int variable_id);

int is_number(const char symbol);

struct bounds *bounds_alloc(size_t var_count);

int bounds_init(struct bounds *bounds, size_t var_count);

int arrays_alloc(struct bounds *bounds, size_t var_count);

void arrays_dealloc(struct bounds *bounds, size_t var_count);

void bounds_deinit(struct bounds *bounds, size_t var_count);

void bounds_dealloc(struct bounds **bounds, size_t var_count);

void set_default_bounds(struct bounds *bounds, size_t var_count);

int is_nan(double value);

int is_plus_inf(double value);

int is_minus_inf(double value);

void print_bounds(struct bounds *bounds, struct generals *generals);

#endif