#include "../label.h"

#ifndef GENERALS_H
#define GENERALS_H

#define MAX_VAR_LEN 256

struct generals{
    char **variables;
    size_t variables_count;
};

int get_variable(const char * var_1, const struct generals *generals);

struct generals *process_generals(char **generals, size_t line_count);

int get_variables(char *line, struct generals *generals);

struct generals *generals_alloc();

int generals_init(struct generals *generals);

void generals_dealloc(struct generals **generals);

void generals_deinit(struct generals *generals);

char *variable_alloc(const size_t var_len, const char *var);

int variable_init(char *new_var, const char *var, const size_t var_len);

int variables_alloc(struct generals *generals, const size_t var_len, const size_t var_count, const char *variable);

void variable_dealloc(char **variable);

void variables_dealloc(char ***variables, const size_t var_count);

void print_generals(const struct generals *generals);

int generals_set_variables(struct generals *generals, char **variables);

int generals_set_variables_count(struct generals *generals, const size_t var_count);

int search_variables(char *line, const struct generals *generals);

#endif