#include "simplex.h"

int simplex(struct processed_file *processed_file, char *output_file_name){
    struct simplex_table *table = NULL;

    if(!processed_file){
        error = POINTER_ERR;
        return 0;
    }

    table = add_variables(processed_file->subject_to, processed_file->generals);

    if(!table){
        return 0;
    }

    if(!iterate(table)){
        goto err;
    }

    if(!check_artificial_variables(table)){
        goto err;
    }

    if(!phase_two_init(table, processed_file->function, processed_file->generals)){
        goto err;
    }

    if(!iterate(table)){
        goto err;
    }

    if(!print_solution(table, processed_file->generals, output_file_name)){
        goto err;
    }

    simplex_table_dealloc(&table);

    return 1;

    err:
        simplex_table_dealloc(&table);
        return 0;
}

int iterate(struct simplex_table *table){
    if(!table){
        error = POINTER_ERR;
        return 0;
    }

    if(!set_base_zero(table)){
        return 0;;
    }

    while(check_non_negative_row(table)){
        if(!replace_base(table)){
            return 0;
        }
    }

    return 1;
}

struct simplex_table *add_variables(struct subject_to *subject_to, struct generals *generals){
    size_t additional_variables_count;
    struct function *help_function = NULL;
    struct subject_to *help_subject_to = NULL;
    struct simplex_table *table = NULL;
    if(!subject_to || !generals){
        error = POINTER_ERR;
        return NULL;
    }

    additional_variables_count = count_additional_variables(subject_to);

    help_function = function_alloc(additional_variables_count + generals->variables_count);

    if (!help_function){
        return 0;
    }

    help_subject_to = subject_to_alloc(additional_variables_count + generals->variables_count, subject_to->line_count);

    if (!help_subject_to){
        function_dealloc(&help_function);
        return 0;
    }

    if (!copy_sub_matrix(help_subject_to, subject_to, additional_variables_count + generals->variables_count, generals->variables_count)){
        subject_to_dealloc(&help_subject_to);
        function_dealloc(&help_function);
        return 0;
    }

    table = set_additional_variables(help_subject_to, help_function, generals, additional_variables_count);

    if (!table){
        subject_to_dealloc(&help_subject_to);
        function_dealloc(&help_function);
        return NULL;
    }
    
    subject_to_dealloc(&help_subject_to);
    function_dealloc(&help_function);
    return table;
}

struct simplex_table *set_additional_variables(struct subject_to *help_subject_to, struct function *help_function, struct generals *generals, size_t additional_variables_count){
    size_t i, current_var_count, artificial_vars_count;
    size_t *base = NULL;
    size_t *artificial_vars_ids = NULL;
    struct simplex_table *table = NULL;

    if(!help_subject_to || !help_function || !generals){
        error = POINTER_ERR;
        return NULL;
    }

    base = calloc(sizeof(size_t), help_subject_to->line_count);

    if(!base){
        error = MEMORY_ERR;
        return NULL;
    }

    artificial_vars_ids = calloc(sizeof(size_t), additional_variables_count);

    if(!artificial_vars_ids){
        free(base);
        error = MEMORY_ERR;
        return NULL;
    }

    artificial_vars_count = 0;
    current_var_count = 0;

    for (i = 0; i < help_subject_to->line_count; ++i){
        if (help_subject_to->operators[i] == '<'){

            help_subject_to->coefs[i][generals->variables_count + current_var_count] = 1.0;
            base[i] = generals->variables_count + current_var_count;

            ++current_var_count;
            continue;
        }

        if (help_subject_to->operators[i] == '='){

            help_subject_to->coefs[i][generals->variables_count + current_var_count] = 1.0;
            help_function->coefs[generals->variables_count + current_var_count] = 1.0;
            base[i] = generals->variables_count + current_var_count;

            artificial_vars_ids[artificial_vars_count] = generals->variables_count + current_var_count;
            ++artificial_vars_count;
            ++current_var_count;
            continue;
        }

        if (help_subject_to->operators[i] == '>'){

            help_subject_to->coefs[i][generals->variables_count + current_var_count] = -1.0;
            ++current_var_count;

            help_subject_to->coefs[i][generals->variables_count + current_var_count] = 1.0;
            help_function->coefs[generals->variables_count + current_var_count] = 1.0;
            base[i] = generals->variables_count + current_var_count;

            artificial_vars_ids[artificial_vars_count] = generals->variables_count + current_var_count;
            ++artificial_vars_count;   
            ++current_var_count;
            continue;
        } 
    }

    if(artificial_vars_count == 0){
        free(artificial_vars_ids);
        artificial_vars_ids = NULL;
    }

    table = simplex_table_alloc(help_function, help_subject_to, generals->variables_count + current_var_count, base, artificial_vars_ids, artificial_vars_count);

    if (!table){
        free(base);
        free(artificial_vars_ids);
        return NULL;
    }
    
    free(artificial_vars_ids);
    free(base);
    return table;
}

size_t count_additional_variables(struct subject_to *subject_to){
    size_t i, additional_variables_count;

    additional_variables_count = 0;
    for (i = 0; i < subject_to->line_count; ++i){
        if (subject_to->operators[i] == '<'){
            ++additional_variables_count;
            continue;
        }

        if (subject_to->operators[i] == '='){
            ++additional_variables_count;
            continue;
        }

        if (subject_to->operators[i] == '>'){
            additional_variables_count += 2;
            continue;
        } 
    }

    return additional_variables_count;
}


struct simplex_table *simplex_table_alloc(struct function *function, struct subject_to *subject_to, size_t var_count, size_t *base_ids, size_t *artificial_vars_ids, size_t artificial_vars_count){
    struct simplex_table *new_table = NULL;

    if (!function || !subject_to || !base_ids || var_count == 0){
        error = SYNTAX_ERR;
        return NULL;
    }

    new_table = malloc(sizeof(*new_table));

    if(!new_table){
        error = MEMORY_ERR;
        return NULL;
    }

    if(!simplex_table_init(new_table, function, subject_to, var_count, base_ids, artificial_vars_ids, artificial_vars_count)){
        free(new_table);
        return NULL;
    }

    return new_table;
}

int simplex_table_init(struct simplex_table *table, struct function *function, struct subject_to *subject_to, size_t var_count, size_t *base_ids, size_t *artificial_vars_ids, size_t artificial_vars_count){
    size_t i;

    if(!table || !function || !subject_to || !base_ids || var_count == 0){
        error = POINTER_ERR;
        return 0;
    }

    table->cols = var_count + 1;
    table->rows = subject_to->line_count + 1;
    table->base = calloc(sizeof(size_t), subject_to->line_count);

    if (!table->base){
        error = MEMORY_ERR;
        return 0;
    }

    if(artificial_vars_count == 0){
        table->artificial_vars_ids = NULL;
        table->artificial_vars_count = 0;
    }

    else{
        table->artificial_vars_ids = calloc(sizeof(size_t), artificial_vars_count);
        if(!table->artificial_vars_ids){
            free(table->base);
            error = MEMORY_ERR;
            return 0;
        }

        memcpy(table->artificial_vars_ids, artificial_vars_ids, sizeof(size_t) * artificial_vars_count);
        table->artificial_vars_count = artificial_vars_count;
    }

    table->coefs = calloc(sizeof(double *), table->rows);

    if (!table->coefs){
        free(table->artificial_vars_ids);
        table->artificial_vars_ids = NULL;
        free(table->base);
        table->base = NULL;
        error = MEMORY_ERR;
        return 0;
    }

    if(!rows_alloc(table->coefs, table->rows, table->cols)){
        free(table->artificial_vars_ids);
        table->artificial_vars_ids = NULL;
        free(table->base);
        table->base = NULL;
        free(table->coefs);
        table->coefs = NULL;
        return 0;
    }

    for (i = 0; i < subject_to->line_count; ++i){
        memcpy(table->coefs[i], subject_to->coefs[i], sizeof(double) * var_count);
        table->coefs[i][var_count] = subject_to->right_sides[i];
    }

    memcpy(table->base, base_ids, sizeof(size_t) * subject_to->line_count);
    memcpy(table->coefs[subject_to->line_count], function->coefs, sizeof(double) * var_count);

    return 1;
}

int rows_alloc(double **coefs, size_t rows, size_t cols){
    size_t i;

    if(!coefs || rows == 0 || cols == 0){
        error = POINTER_ERR;
        return 0;
    }

    for (i = 0; i < rows; ++i){
        coefs[i] = calloc(sizeof(double), cols);

        if(!coefs[i]){
            rows_dealloc(coefs, rows);
            error = MEMORY_ERR;
            return 0;
        }
    }

    return 1;
}

void rows_dealloc(double **coefs, size_t rows){
    size_t i;

    if(!coefs){
        return;
    }

    for (i = 0; i < rows; ++i){
        free(coefs[i]);
        coefs[i] = NULL;
    }

    return;
}

void simplex_table_deinit(struct simplex_table *table){
    if(!table){
        return;
    }

    rows_dealloc(table->coefs, table->rows);

    free(table->base);
    table->base= NULL;

    free(table->coefs);
    table->coefs = NULL;

    free(table->artificial_vars_ids);
    table->artificial_vars_ids = NULL;

    table->artificial_vars_count = 0;
    table->rows = 0;
    table->cols = 0;

    return;
}

void simplex_table_dealloc(struct simplex_table **table){
    if(!table || !(*table)){
        return;
    }

    simplex_table_deinit(*table);
    free(*table);
    *table = NULL;
    return;
}

void print_table(const struct simplex_table *table){
    size_t i, j;

    i = j = 0;
    for (i = 0; i < table->rows; ++i){
        for (j = 0; j < table->cols; ++j){
            
            printf("%f  ", table->coefs[i][j]);
        }
        j = 0;
        printf("\n");
    }
}

int set_base_zero(struct simplex_table *table){
    size_t i, base_id;
    double mul_value;
    if(!table){
        error = POINTER_ERR;
        return 0;
    }

    for (i = 0; i < table->rows - 1; ++i){
        base_id = table->base[i];
        mul_value = table->coefs[table->rows - 1][base_id] * -1.0;
        if(!sum_rows(table, i, table->rows - 1, mul_value)){
            return 0;
        }
    }

    return 1;
}

int sum_rows(struct simplex_table *table, size_t src_row, size_t dst_row, double multiplier){
    size_t i;

    if(!table){
        error = POINTER_ERR;
        return 0;
    }

    for (i = 0; i < table->cols; ++i){
        table->coefs[dst_row][i] += table->coefs[src_row][i] * multiplier;
    }
    
    return 1;
}

int check_non_negative_row(struct simplex_table *table){
    size_t i;

    if(!table){
        error = POINTER_ERR;
        return -1;
    }

    for (i = 0; i < table->cols - 1; i++){
        if (table->coefs[table->rows - 1][i] < 0){
            return 1;
        }
    }

    return 0;   
}

int replace_base(struct simplex_table *table){
    int pivot_col_id;
    size_t i, pivot_row_id;
    double ration, current_ration;

    if(!table){
        error = POINTER_ERR;
        return 0;
    }

    pivot_col_id = get_pivot_col(table);
    if(pivot_col_id == -1){
        return 0;
    }

    ration = PLUSINF;
    for (i = 0; i < table->rows - 1; ++i){
        current_ration = table->coefs[i][table->cols - 1] / table->coefs[i][pivot_col_id];
        if(is_plus_inf(current_ration)){
            continue;
        }
        
        if(current_ration >= 0 && current_ration < ration){
            ration = current_ration;
            pivot_row_id = i;
        }
    }

    if (is_plus_inf(ration)){
        error = FUNC_BOUND_ERR;
        return 0;
    }
    
    table->base[pivot_row_id] = pivot_col_id;

    if(!divide_row(table, pivot_row_id, table->coefs[pivot_row_id][pivot_col_id])){
        return 0;
    }

    for (i = 0; i < table->rows; ++i){
        if(i == pivot_row_id){
            continue;
        }

        if(!sum_rows(table, pivot_row_id, i, -table->coefs[i][pivot_col_id])){
            return 0;
        }
    }

    return 1;
}


int divide_row(struct simplex_table *table, size_t row_id, double divisor){
    size_t i;

    if (!table){
        error = POINTER_ERR;
    }

    for ( i = 0; i < table->cols; i++){
        table->coefs[row_id][i] = table->coefs[row_id][i] / divisor;
    }
    
    return 1;
}

int get_pivot_col(struct simplex_table *table){
    size_t i;
    int pivot_col_id;
    double value_pivot_col;

    if(!table){
        error = POINTER_ERR;
        return -1;
    }

    pivot_col_id = -2;
    value_pivot_col = 0;
    for (i = 0; i < table->cols - 1; ++i){
        if(value_pivot_col > table->coefs[table->rows - 1][i]){
            value_pivot_col = table->coefs[table->rows - 1][i];
            pivot_col_id = i;
        }
    }
    
    return pivot_col_id;
}

int is_artificial(size_t id, size_t *artificial_variables_ids, size_t artificial_variables_count){
    size_t i;

    if (!artificial_variables_ids){
        return 0;
    }
    
    for (i = 0; i < artificial_variables_count; ++i){
        if (id == artificial_variables_ids[i]){
            return 1;
        }
    }

    return 0;
    
}

int check_artificial_variables(const struct simplex_table *table){
    size_t i;

    if(!table){
        error = POINTER_ERR;
        return 0;
    }

    for (i = 0; i < table->rows - 1; ++i){
        if (is_artificial(table->base[i], table->artificial_vars_ids, table->artificial_vars_count)){
            error = SOLUTION_ERR;
            return 0;
        }
    }

    return 1;
}

int phase_two_init(struct simplex_table *table, struct function *original_function, const struct generals *generals){
    size_t i, j;

    if (!table || !original_function || !generals){
        error = POINTER_ERR;
        return 0;
    }

    for (i = 0; i < table->cols - 1; ++i){
        if(is_artificial(i, table->artificial_vars_ids, table->artificial_vars_count)){

            for (j = 0; j < table->rows; ++j){
                table->coefs[j][i] = 0.0;
            }
        }

        if(i < generals->variables_count){
            table->coefs[table->rows - 1][i] = -1 * original_function->coefs[i];
        }
        else{
            table->coefs[table->rows - 1][i] = 0;
        }
    }  

    return 1;
}

int is_in_base(const struct simplex_table *table, size_t id, size_t *row){
    size_t i;

    for (i = 0; i < table->rows - 1; ++i){
        if (table->base[i] == id){
            *row = i;
            return 1;
        }
    }

    return 0; 
}

int print_solution(const struct simplex_table *table, const struct generals *generals, char *output_file_path){
    size_t i;
    size_t row[1];
    FILE *output_file;

    if(!table || !generals){
        error = POINTER_ERR;
        return 0;
    }

    if(!output_file_path){
        for (i = 0; i < generals->variables_count; ++i){
            if (is_in_base(table, i, row)){
                printf("%s = %f\n", generals->variables[i], table->coefs[*row][table->cols - 1]);
                continue;
            }

            printf("%s = %f\n", generals->variables[i], 0.000);
        }  
    }

    else{

        output_file = fopen(output_file_path, "w");
        if(!output_file){
            error = OUTPUT_FILE_ERR;
            return 0;
        }

        for (i = 0; i < generals->variables_count; ++i){
            if (is_in_base(table, i, row)){
                fprintf(output_file, "%s = %f\n", generals->variables[i], table->coefs[*row][table->cols - 1]);
                continue;
            }

            fprintf(output_file, "%s = %f\n", generals->variables[i], 0.000);
        } 
        fclose(output_file);
    }

    return 1;
}