#include "../file_proces/file_proces.h"

#ifndef SIMPLEX_H
#define SIMPLEX_H

struct simplex_table{
    double **coefs;
    size_t *base;
    size_t rows;
    size_t cols;
    size_t *artificial_vars_ids;
    size_t artificial_vars_count;
};

int simplex(struct processed_file *processed_file, char *output_file_name);

int iterate(struct simplex_table *table);

struct simplex_table *add_variables(struct subject_to *subject_to, struct generals *generals);

struct simplex_table *set_additional_variables(struct subject_to *help_subject_to, struct function *help_function, struct generals *generals, size_t additional_variables_count);

size_t count_additional_variables(struct subject_to *subject_to);

struct simplex_table *simplex_table_alloc(struct function *function, struct subject_to *subject_to, size_t var_count, size_t *base_ids, size_t *artificial_vars_ids, size_t artificial_vars_count);

int simplex_table_init(struct simplex_table *table, struct function *function, struct subject_to *subject_to, size_t var_count, size_t *base_ids, size_t *artificial_vars_ids, size_t artificial_vars_count);

int rows_alloc(double **coefs, size_t rows, size_t cols);

void rows_dealloc(double **coefs, size_t rows);

void simplex_table_deinit(struct simplex_table *table);

void simplex_table_dealloc(struct simplex_table **table);

void print_table(const struct simplex_table *table);

int set_base_zero(struct simplex_table *table);

int sum_rows(struct simplex_table *table, size_t src_row, size_t dst_row, double multiplier);

int check_non_negative_row(struct simplex_table *table);

int get_pivot_col(struct simplex_table *table);

int divide_row(struct simplex_table *table, size_t row_id, double divisor);

int replace_base(struct simplex_table *table);

int is_artificial(size_t id, size_t *artificial_variables_ids, size_t artificial_variables_count);

int check_artificial_variables(const struct simplex_table *table);

int phase_two_init(struct simplex_table *table, struct function *original_function, const struct generals *generals);

int is_in_base(const struct simplex_table *table, size_t id, size_t *row);

int print_solution(const struct simplex_table *table, const struct generals *generals, char *output_file_path);

#endif