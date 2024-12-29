#include "../function/function.h"

#ifndef SUBJECT_TO_H
#define SUBJECT_TO_H

#define TEXT_SPLITTER ":"
#define LOWER '<'
#define HIGHER '>'
#define EQUAL '='

struct subject_to{
    double **coefs;
    size_t line_count;
    char *operators;
    double *right_sides;
};

struct subject_to *process_subject_to(char **subject_to, size_t line_count, const struct generals *generals);

int parse_equation(struct subject_to *subject_to, char *equation, const size_t id, const struct generals *generals);

struct subject_to *subject_to_alloc(size_t coef_count, size_t line_count);

int subject_to_init(struct subject_to *subject_to, size_t coef_count, size_t line_count);

void coefs_dealloc(double **coefs, size_t line_count);

int coefs_alloc(double **coefs, size_t coef_count, size_t line_count);

void subject_to_deinit(struct subject_to *subject_to);

void subject_to_dealloc(struct subject_to **subject_to);

int control_subject_to(char *subject_to);

void subj_print_coefs(struct subject_to *subject_to, struct generals *generals);

#endif