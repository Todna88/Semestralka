#include "subject_to.h"

struct subject_to *process_subject_to(char **subject_to, const size_t line_count, const struct generals *generals){
    struct subject_to *processed_subject_to = NULL;
    char *equation;
    size_t i, j;

    if(!subject_to || !(*subject_to) || !generals){
        error = POINTER_ERR;
        return NULL;
    }

    processed_subject_to = subject_to_alloc(generals->variables_count, line_count);

    if(!processed_subject_to){
        return NULL;
    }

    i = 0;
    j = 0;
    for (i = 0; i < line_count; ++i){

        if(!check_line_syntax(subject_to[i])){
            subject_to_dealloc(&processed_subject_to);
            return NULL;  
        }

        equation = strtok(subject_to[i], TEXT_SPLITTER);

        while (equation != NULL){

            if(j == 1){
                break;
            }
            equation = strtok(NULL, TEXT_SPLITTER);
            ++j;
        }

        if(!parse_equation(processed_subject_to, equation, i, generals)){
            subject_to_dealloc(&processed_subject_to);
            return NULL;
        }

        j = 0;
    }

    return processed_subject_to;
}

int parse_equation(struct subject_to *subject_to, char *equation, const size_t id, const struct generals *generals){
    char *token, *ptr;
    const char lower[2] = "<";
    const char higher[2] = ">";
    const char equal[2] = "=";

    if (!subject_to || !equation){
        error = POINTER_ERR;
        goto err;
    }

    delete_spaces(equation);
    if(!control_subject_to(equation)){
        return 0;
    }

    if(strchr(equation, LOWER)){
        token = strtok(equation, lower);

        while (token != NULL){
            if (search_variables(token, generals)){
                if(!parse_artithmetic_expression(token, subject_to->coefs[id], generals)){
                    goto err;
                }
            }

            else{     
                if (*token == EQUAL){
                    subject_to->right_sides[id] = strtod(token + EQUAL_LEN, &ptr);
                }
                else{
                    subject_to->right_sides[id] = strtod(token, &ptr);
                }

                if(*ptr != ENDING_CHAR){
                    error = SYNTAX_ERR;
                    goto err;
                }
            }
            
            token = strtok(NULL, lower);
        }

        if(subject_to->right_sides[id] < 0){
            subject_to->operators[id] = HIGHER;
            subject_to->right_sides[id] = subject_to->right_sides[id] * NEGATIVE_MILTIPLIER;
            switch_signs(subject_to->coefs[id], generals->variables_count);
        }

        else{
            subject_to->operators[id] = LOWER;
        }

        return 1;
    }

    if(strchr(equation, HIGHER)){
        token = strtok(equation, higher);

        while (token != NULL){
            if (search_variables(token, generals)){
                if(!parse_artithmetic_expression(token, subject_to->coefs[id], generals)){
                    goto err;
                }
            }

            else{
                if (*token == EQUAL){
                    subject_to->right_sides[id] = strtod(token + EQUAL_LEN, &ptr);
                }
                else{
                    subject_to->right_sides[id] = strtod(token, &ptr);
                }
            }
            
            token = strtok(NULL, higher);
        }

        if(subject_to->right_sides[id] < 0){
            subject_to->operators[id] = LOWER;
            subject_to->right_sides[id] = subject_to->right_sides[id] * NEGATIVE_MILTIPLIER;
            switch_signs(subject_to->coefs[id], generals->variables_count);
        }

        else{
            subject_to->operators[id] = HIGHER;
        }

        return 1;      
    }

    if(strchr(equation, EQUAL)){
        token = strtok(equation, equal);

        while (token != NULL){
            if (search_variables(token, generals)){
                if(!parse_artithmetic_expression(token, subject_to->coefs[id], generals)){
                    goto err;
                }
            }

            else{
                if (*token == EQUAL){
                    subject_to->right_sides[id] = strtod(token + EQUAL_LEN, &ptr);
                }
                else{
                    subject_to->right_sides[id] = strtod(token, &ptr);
                }
            }
            
            token = strtok(NULL, higher);
        }

        subject_to->operators[id] = EQUAL;
        return 1;      
    }

    error = SYNTAX_ERR;
    err:
        return 0;  
}

struct subject_to *subject_to_alloc(const size_t coef_count, const size_t line_count){
    struct subject_to *new_subject_to = NULL;

    new_subject_to = malloc(sizeof(*new_subject_to));

    if (!new_subject_to){
        error = MEMORY_ERR;
        return NULL;
    }

    if(subject_to_init(new_subject_to, coef_count, line_count) == 0){
        free(new_subject_to);
        return NULL;
    }

    return new_subject_to;
}

int subject_to_init(struct subject_to *subject_to, const size_t coef_count, const size_t line_count){
    if(!subject_to){
        error = POINTER_ERR;
        return 0;
    }

    subject_to->coefs = calloc(sizeof(double *), line_count);
    if(!subject_to->coefs){
        error = MEMORY_ERR;
        return 0;
    }

    if(!coefs_alloc(subject_to->coefs, coef_count, line_count)){
        free(subject_to->coefs);
        return 0;
    }

    subject_to->operators = calloc(sizeof(char), line_count);

    if(!subject_to->operators){
        coefs_dealloc(subject_to->coefs, line_count);
        free(subject_to->coefs);
        error = MEMORY_ERR;
        return 0;
    }

    subject_to->right_sides = calloc(sizeof(double), line_count);

    if(!subject_to->right_sides){
        coefs_dealloc(subject_to->coefs, line_count);
        free(subject_to->coefs);
        free(subject_to->operators);
        error = MEMORY_ERR;
        return 0;
    }

    subject_to->line_count = line_count;

    return 1;
}

int coefs_alloc(double **coefs, const size_t coef_count, const size_t line_count){
    size_t i;

    if (!coefs){
        error = POINTER_ERR;
        return 0;
    }

    i = 0;
    for (i = 0; i < line_count; i++){
        coefs[i] = calloc(sizeof(double), coef_count);
        
        if (!coefs[i]){
            coefs_dealloc(coefs, line_count);
            error = MEMORY_ERR;
            return 0;
        }  
    }

    return 1;
}

void coefs_dealloc(double **coefs, const size_t line_count){
    size_t i;

    if (!coefs){
        return;
    }

    i = 0;
    for (i = 0; i < line_count; i++){
        free(coefs[i]);
    }

    return;
}

void subject_to_deinit(struct subject_to *subject_to){
    if(!subject_to){
        return;
    }

    coefs_dealloc(subject_to->coefs, subject_to->line_count);

    free(subject_to->coefs);
    subject_to->coefs = NULL;
    free(subject_to->operators);
    subject_to->operators = NULL;
    free(subject_to->right_sides);
    subject_to->right_sides = NULL;

    return;
}

void subject_to_dealloc(struct subject_to **subject_to){
    if(!subject_to || !(*subject_to)){
        return;
    }

    subject_to_deinit(*subject_to);
    free(*subject_to);
    *subject_to = NULL;
    return;
}

int control_subject_to(char *subject_to){
    const char *banned_chars = "^/:,";

    if(strpbrk(subject_to, banned_chars)){
        error = SYNTAX_ERR;
        return 0;
    }

    return 1;
}

int copy_sub_matrix(struct subject_to *subject_to, const struct subject_to *smaller_subject_to, const size_t var_count_bigger, const size_t var_count_smaller){
    size_t i;

    if(!subject_to || !smaller_subject_to || (var_count_smaller > var_count_bigger)){
        error = POINTER_ERR;
        return 0;
    }

    for (i = 0; i < subject_to->line_count; ++i){
        memcpy(subject_to->coefs[i], smaller_subject_to->coefs[i], sizeof(double) * var_count_smaller);
    }

    memcpy(subject_to->operators, smaller_subject_to->operators, sizeof(char) * subject_to->line_count);
    memcpy(subject_to->right_sides, smaller_subject_to->right_sides, sizeof(double) * subject_to->line_count);
    
    return 1;
}

int check_line_syntax(const char *line){
    size_t i;
    int j;

    if(!line){
        error = POINTER_ERR;
        return 0;
    }

    i = 0;
    j = 0;
    while (line[i] != ENDING_CHAR){
        if (line[i] == DOUBLE_DOT){
            ++j;
        }
        ++i;
    }

    if (j != 1){
        error = SYNTAX_ERR;
        return 0;
    }

    return 1;
}

void switch_signs(double *coefs, const size_t var_count){
    size_t i;

    if(!coefs || var_count == 0){
        return;
    }

    for (i = 0; i < var_count; ++i){
        coefs[i] = coefs[i] * NEGATIVE_MILTIPLIER;
    }

    return;
}