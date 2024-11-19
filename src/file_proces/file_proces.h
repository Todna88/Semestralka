#include "label/generals/generals.h"

#ifndef FILE_PROCES_H
#define FILE_PROCES_H

struct processed_file{
    char **variables;
    double *function_coefs;
    double **constrait_coef;
    double *bounds;
};

struct divided_file{
    char **generals;
    size_t generals_line_count;
    char **bounds;
    size_t bounds_line_count;
    char *function;
    char * function_type;
    char **subject_to;
    size_t subject_to_line_count;
};

#define DEFAUlT_DIVIDED_FILE {NULL, 0, NULL, 0, NULL, NULL, NULL, 0}

#define MIN_TYPE "Minimalize"

#define MAX_TYPE "Maximalize"

int process_file(FILE *file);

struct divided_file *divide_file(FILE *file);

struct divided_file *divided_file_alloc();

int divided_file_init(struct divided_file *divided_file);

int subject_to_alloc(struct divided_file *file, const size_t line_len, const size_t line_count, const char * line);

int function_alloc(struct divided_file *file, const size_t line_len, char *function_type, const char *line);

int bounds_alloc(struct divided_file *file, const size_t line_len, const size_t line_count, const char *line);

int gen_alloc(struct divided_file *file, const size_t line_len, const size_t line_count, const char *line);

char *line_alloc(const size_t line_len, const char *line);

int line_init(char *new_line, const char *line, const size_t line_len);

void line_dealloc(char **line);

void divided_file_dealloc(struct divided_file **devided_file);

void divided_file_deinit(struct divided_file *devided_file);

void label_dealloc(char ***label, const size_t line_count);

#endif