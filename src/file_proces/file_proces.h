#include "label/generals/generals.h"
#include "label/function/function.h"
#include "label/subject_to/subject_to.h"
#include "label/bounds/bounds.h"

#ifndef FILE_PROCES_H
#define FILE_PROCES_H

struct processed_file{
    struct generals *generals;
    struct function *function;
    struct subject_to *subject_to;
    struct bounds *bounds;
};

struct divided_file{
    char **generals;
    size_t generals_line_count;
    char **bounds;
    size_t bounds_line_count;
    char *function;
    char *function_type;
    char **subject_to;
    size_t subject_to_line_count;
};

#define DEFAUlT_DIVIDED_FILE {NULL, 0, NULL, 0, NULL, NULL, NULL, 0}

struct processed_file *process_file(FILE *file);

struct divided_file *divide_file(FILE *file);

struct divided_file *divided_file_alloc();

int divided_file_init(struct divided_file *divided_file);

int subj_to_alloc(struct divided_file *file, const size_t line_len, const size_t line_count, const char * line);

int func_alloc(struct divided_file *file, const size_t line_len, char *function_type, const char *line);

int bound_alloc(struct divided_file *file, const size_t line_len, const size_t line_count, const char *line);

int gen_alloc(struct divided_file *file, const size_t line_len, const size_t line_count, const char *line);

char *line_alloc(const size_t line_len, const char *line);

int line_init(char *new_line, const char *line, const size_t line_len);

void line_dealloc(char **line);

void divided_file_dealloc(struct divided_file **devided_file);

void divided_file_deinit(struct divided_file *devided_file);

void label_dealloc(char ***label, const size_t line_count);

int check_mandatory_labels(struct divided_file *file);

int check_end_file(FILE *file);

struct processed_file *processed_file_alloc(struct generals *generals, struct function *function, struct subject_to *subject_to, struct bounds *bounds);

int processed_file_init(struct processed_file *processed_file, struct generals *generals, struct function *function, struct subject_to *subject_to, struct bounds *bounds);

void processed_file_dealloc(struct processed_file **processed_file);

void processed_file_deinit(struct processed_file *processed_file);

void check_unused_variables(const struct function *function, const struct subject_to *subject_to, const struct generals *generals);

#endif