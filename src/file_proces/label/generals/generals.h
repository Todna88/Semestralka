#include "../label.h"

#include <stdio.h>

#ifndef GENERALS_H
#define GENERALS_H

struct generals{
    char **variables;
    size_t variables_count;
};

struct generals *process_generals(FILE *stream);

struct generals *get_variables(char *line);

struct generals *generals_alloc(char **variables, size_t var_count);

int generals_init(struct generals *generals, char **variables, size_t var_count);

void generals_dealloc(struct generals **generals);

void generals_deinit(struct generals *generals);

char *variable_alloc(size_t size);

char **variables_alloc(size_t size);

void variable_dealloc(char **variable);

void variables_dealloc(char ***variables, size_t var_size);

size_t var_count(char *line);

void print_generals(struct generals *generals);
#endif