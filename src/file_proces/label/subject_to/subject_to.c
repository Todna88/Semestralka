#include "subject_to.h"

struct subject_to *process_subject_to(char **subject_to, size_t line_count, const struct generals *generals){
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
        check_unused_variables(processed_subject_to->coefs[i], generals);
    }

    return processed_subject_to;
}

int parse_equation(struct subject_to *subject_to, char *equation, const size_t id, const struct generals *generals){
    char *token, *ptr;
    char lower[2] = "<";
    char higher[2] = ">";
    char equal[2] = "=";

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
                if (*token == '='){
                    subject_to->right_sides[id] = strtod(token + 1, &ptr);
                }
                else{
                    subject_to->right_sides[id] = strtod(token, &ptr);
                }
            }
            
            token = strtok(NULL, lower);
        }

        subject_to->operators[id] = LOWER;
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
                if (*token == '='){
                    subject_to->right_sides[id] = strtod(token + 1, &ptr);
                }
                else{
                    subject_to->right_sides[id] = strtod(token, &ptr);
                }
            }
            
            token = strtok(NULL, higher);
        }

        subject_to->operators[id] = HIGHER;
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
                if (*token == '='){
                    subject_to->right_sides[id] = strtod(token + 1, &ptr);
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

struct subject_to *subject_to_alloc(size_t coef_count, size_t line_count){
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

int subject_to_init(struct subject_to *subject_to, size_t coef_count, size_t line_count){
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

int coefs_alloc(double **coefs, size_t coef_count, size_t line_count){
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

void coefs_dealloc(double **coefs, size_t line_count){
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
    char *banned_chars = "^/:,";

    if(strpbrk(subject_to, banned_chars)){
        error = SYNTAX_ERR;
        return 0;
    }

    return 1;
}

void subj_print_coefs(struct subject_to *subject_to, struct generals *generals){
    size_t i, j;

    if (!subject_to || !generals){
        return;
    }

    for (i = 0; i < subject_to->line_count; ++i){
        for (j = 0; j < generals->variables_count; ++j){
            printf("Subject to coeficient of variable %s in line %lu is %f\n", generals->variables[j], i ,subject_to->coefs[i][j]);
        } 
    }
    
    return;   
}