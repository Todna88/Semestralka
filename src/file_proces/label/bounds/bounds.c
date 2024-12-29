#include "bounds.h"

#include <strings.h>

struct bounds *process_bounds(char **bounds, size_t line_count, const struct generals *generals){
    struct bounds *processed_bounds = NULL;
    size_t i;

    if(!generals){
        error = POINTER_ERR;
        return NULL;
    }

    processed_bounds = bounds_alloc(generals->variables_count);

    if(!processed_bounds){
        return NULL;
    }

    if (line_count == 0){
        set_default_bounds(processed_bounds, generals->variables_count);
        return processed_bounds;
    }
    
    set_default_bounds(processed_bounds, generals->variables_count);
    
    for (i = 0; i < line_count; ++i){
        if (!parse_bound(processed_bounds, bounds[i], generals)){
            bounds_dealloc(&processed_bounds, generals->variables_count);
            return NULL;
        } 
    }

    return processed_bounds;
}

int parse_bound(struct bounds *bounds, char *line, const struct generals* generals){
    const char equal[2] = "=";
    char *token;
    double values[3];
    char constr[3] = {0, 0, 0};
    size_t i, equal_flag;
    int variable_id = -1;

    values[0] = NAN;
    values[1] = NAN;
    values[2] = NAN;

    if (!line || !generals || !bounds){
        error = POINTER_ERR;
        return 0;
    }

    if(!strchr(line, EQUAL)){
        if (!parse_free(bounds, line, generals)){
            return 0;
        }

        return 1;
    }

    token = strtok(line, equal);

    equal_flag = 0;
    i = 0;
    while(token != NULL){
        delete_spaces(token);

        if(i > 2){
            error = SYNTAX_ERR;
            return 0;
        }

        if (is_number(token[0])){
            if(!parse_num(variable_id, &equal_flag, values, constr, token, i)){
                return 0;
            }
        }

        else{
            if(!parse_var(&variable_id, &equal_flag, constr, token, i, generals, values)){
                return 0;
            }
        }
        
        token = strtok(NULL, equal);
        ++i;
    }

    if(!check_syntax(variable_id, values, constr, 3)){
        return 0;
    }
    
    memcpy(bounds->bounds[variable_id], constr, sizeof(char) * BOUNDS_ARRAY_COUNT);

    cpy_values(bounds, values, variable_id);

    return 1;  
}

void cpy_values(struct bounds *bounds, double *values, int variable_id){
    size_t i, j;
    double equivalent_values[2];

    equivalent_values[0] = NAN;
    equivalent_values[1] = NAN;

    i = j = 0;
    for (i = 0; i < 3; ++i){
        if(is_nan(values[i])){
            continue;
        }

        equivalent_values[j] = values[i];
        ++j;
    }

    memcpy(bounds->values[variable_id], equivalent_values, sizeof(double) * VALUES_ARRAY_COUNT);
    
    return;
}

int check_syntax(int variable_id, double *values, char *constr, size_t values_len){
    size_t i;

    if (!values || !constr){
        error = POINTER_ERR;
        return 0;
    }

    if(variable_id == -1){
        return 0;
    }

    if(is_nan(values[0]) && is_nan(values[1]) && is_nan(values[2])){
        error = SYNTAX_ERR;
        return 0;
    }

    i = 0;
    for (i = 0; i < values_len - 1; ++i){
        if(!is_nan(values[i]) && !is_nan(values[i + 1])){
            error = SYNTAX_ERR;
            return 0;
        }
    }

    if(constr[0] == '>' && is_plus_inf(values[0])){
        error = SYNTAX_ERR;
        return 0;
    }

    if(constr[0] == '<' && is_minus_inf(values[0])){
        error = SYNTAX_ERR;
        return 0;
    }

    if(check_operator(constr[0]) && check_operator(constr[1])){
        if(constr[0] == constr[1]){
            error = SYNTAX_ERR;
            return 0;
        }

        if(constr[0] == '>' && (values[0] > values[2])){
            error = SYNTAX_ERR;
            return 0;
        }

        if(constr[0] == '<' && (values[0] < values[2])){
            error = SYNTAX_ERR;
            return 0;
        }
    }
    
    return 1;
}

int parse_var(int *variable_id, size_t *equal_flag, char *constr, char *token, int i, const struct generals *generals, double *values){
    size_t j;
    char *variable_name = NULL;

    if(!variable_id || !equal_flag || !constr || !token || !generals || !values){
        error = POINTER_ERR;
        return 0;
    }

    j = 0;
    while ((token[j] != '\0') && !check_operator(token[j])){
        ++j;
    }
    
    if(j == 0){
        error = SYNTAX_ERR;
        return 0;
    }

    variable_name = calloc(sizeof(char), j + 1);
    if (!variable_name){
        error = MEMORY_ERR;
        return 0;
    }
    
    strncpy(variable_name, token, j);

    *variable_id = get_variable(variable_name, generals);
    if(*variable_id == -1){
        if(!strcasecmp(variable_name, "infinity") || !strcasecmp(variable_name, "inf")){
            if(!parse_num(*variable_id, equal_flag, values, constr, token, i)){
                free(variable_name);
                return 0;
            }

            free(variable_name);
            return 1;
        }

        printf("Unknown variable \'%s\'!\n", variable_name);
        free(variable_name);
        error = VARIABLE_ERR;
        return 0;
    }

    free(variable_name);

    if(!parse_oper(token[j], constr, i, equal_flag, *variable_id)){
        return 0;
    }

    return 1;
}

int parse_oper(char last_char, char *constr, int i, size_t *equal_flag, int variable_id){
    if (!constr || !equal_flag){
        error = POINTER_ERR;
        return 0;
    }

    if (!last_char){
        if (i == 0){
            *equal_flag = 1;
            constr[i] = EQUAL;
        }

        else if(i == 2 && (*equal_flag) == 1){
            error = SYNTAX_ERR;
            return 0;
        }
    }

    else{
        if((i == 1 && (*equal_flag) == 1) || i == 2){
            error = SYNTAX_ERR;
            return 0;
        }

        if (variable_id == -1){
            switch_operator(&last_char);
            if (!last_char){
                error = SYNTAX_ERR;
                return 0;
            }

            constr[i] = last_char;
        }

        else{
            if(!check_operator(last_char)){
                error = SYNTAX_ERR;
                return 0;
            }

            constr[i] = last_char;
        }
    }

    return 1;   
}

int parse_num(int variable_id, size_t *equal_flag, double *values, char *constr, char *token, int i){
    char *ptr;

    if(!equal_flag || !constr || !token || !values){
        error = POINTER_ERR;
        return 0;
    }

    values[i] = strtod(token, &ptr);

    if(!parse_oper(ptr[0], constr, i, equal_flag, variable_id)){
        return 0;
    }

    return 1; 
}

int check_operator(char operator){
    if(operator == '>' || operator == '<'){
        return 1;
    }

    return 0;
}

void switch_operator(char *operator){
    if (*operator == '<'){
        *operator =  '>';
        return;
    }

    if(*operator == '>'){
        *operator =  '<';
        return;
    }

    *operator = 0;
    return;
}

int parse_free(struct bounds *bounds, char *line, const struct generals* generals){
    char *token;
    int variable_id;
    size_t i;

    if (!line || !generals || !bounds){
        error = POINTER_ERR;
        return 0;
    }

    token = strtok(line, SPACE);

    variable_id = get_variable(token, generals);
    if(variable_id == -1){
        printf("Unknown variable \'%s\'!\n", token);
        error = VARIABLE_ERR;
        return 0;
    }

    if(!set_free_var(bounds, variable_id)){
        return 0;
    }

    i = 0;
    while(token != NULL){

        token = strtok(NULL, SPACE);
        ++i;
    }

    if(i > 2){
        error = SYNTAX_ERR;
        return 0;
    }

    return 1;
}
int set_free_var(struct bounds *bounds, int variable_id){
    if (!bounds){
        error = POINTER_ERR;
        return 0;
    }
    
    bounds->bounds[variable_id][0] = 0;
    bounds->bounds[variable_id][1] = 0;

    bounds->values[variable_id][0] = NAN;
    bounds->values[variable_id][1] = NAN;
    
    return 1;
}

int is_number(const char symbol){
    if (isdigit(symbol) || (symbol == '-') || (symbol == '+')){
        return 1;
    }
    return 0;
}

struct bounds *bounds_alloc(size_t var_count){
    struct bounds *new_bounds = NULL;

    new_bounds = malloc(sizeof(*new_bounds));

    if (!new_bounds){
        error = MEMORY_ERR;
        return NULL;
    }

    if(bounds_init(new_bounds, var_count) == 0){
        free(new_bounds);
        return NULL;
    }

    return new_bounds;
}

int bounds_init(struct bounds *bounds, size_t var_count){
    if (!bounds || var_count == 0){
        error = POINTER_ERR;
        return 0;
    }

    bounds->bounds = calloc(sizeof(char *), var_count);

    if (!bounds->bounds){
        error = MEMORY_ERR;
        return 0;
    }

    bounds->values = calloc(sizeof(double *), var_count);

    if (!bounds->values){
        free(bounds->bounds);
        error = MEMORY_ERR;
        return 0;
    }

    if (!arrays_alloc(bounds, var_count)){
        free(bounds->bounds);
        free(bounds->values);
        return 0;
    }
    
    return 1;
}

int arrays_alloc(struct bounds *bounds, size_t var_count){
    size_t i;
    if (!bounds || var_count == 0){
        error = POINTER_ERR;
        return 0;
    }

    for (i = 0; i < var_count; ++i){
        bounds->bounds[i] = calloc(sizeof(char), BOUNDS_ARRAY_COUNT);

        if (!bounds->bounds[i]){
            error = MEMORY_ERR;
            arrays_dealloc(bounds, var_count);
            return 0;
        }

        bounds->bounds[i][0] = '>';
        
        bounds->values[i] = calloc(sizeof(double), VALUES_ARRAY_COUNT);

        if (!bounds->values[i]){
            error = MEMORY_ERR;
            arrays_dealloc(bounds, var_count);
            return 0;
        }

        bounds->bounds[i][0] = '0';
    }

    return 1;
}

void arrays_dealloc(struct bounds *bounds, size_t var_count){
    size_t i;
    if (!bounds || var_count == 0){
        return;
    }

    for (i = 0; i < var_count; ++i){
        free(bounds->bounds[i]);
        free(bounds->values[i]);
        bounds->bounds[i] = NULL;
        bounds->values[i] = NULL;
    }

    return;
}

void bounds_deinit(struct bounds *bounds, size_t var_count){
    if (!bounds || var_count == 0){
        return;
    }

    arrays_dealloc(bounds, var_count);
    free(bounds->bounds);
    bounds->bounds = NULL;
    free(bounds->values);
    bounds->values = NULL;

    return;
}

void bounds_dealloc(struct bounds **bounds, size_t var_count){
    if (!bounds || !(*bounds) || var_count == 0){
        return;
    }

    bounds_deinit(*bounds, var_count);
    free(*bounds);
    *bounds = NULL;
    return;
}

void set_default_bounds(struct bounds *bounds, size_t var_count){
    size_t i;
    if (!bounds || var_count == 0){
        return;
    }

    for (i = 0; i < var_count; ++i){
        bounds->bounds[i][0] = '>';
        bounds->bounds[i][1] = 0;

        bounds->values[i][0] = 0;
        bounds->values[i][1] = NAN;
    }

    return;
}

int is_nan(double value){
    if (value != value) {
        return 1;
    }

    return 0;
}

int is_plus_inf(double value){
    union Infinity infinity;
    
    infinity.d = value;

    if (infinity.l == 9218868437227405312){
        return 1;
    }

    return 0;  
}

int is_minus_inf(double value){
    union Infinity infinity;
    
    infinity.d = value;    

    if (infinity.l == -4503599627370496){
        return 1;
    }

    return 0;
}

void print_bounds(struct bounds *bounds, struct generals *generals){
    size_t i,j,k;

    if(!bounds || !generals){
        return;
    }

    k = 0;
    for (i = 0; i < generals->variables_count; ++i){
        for (j = 0; j < 2; ++j){
            if(is_nan(bounds->values[i][j])){
                ++k;
                continue;
            }

            printf("Bound of variable %s is %s %c %f\n", generals->variables[i], generals->variables[i], bounds->bounds[i][j], bounds->values[i][j]);
        }

        if (k == j){
            printf("Variable %s is free!\n", generals->variables[i]);
        }
        k = 0;    
    } 
}