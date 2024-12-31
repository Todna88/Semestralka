#include "generals.h"

#include <ctype.h>

struct generals *process_generals(char **generals, const size_t line_count){
    struct generals *processed_generals = NULL;
    size_t i;

    processed_generals = generals_alloc();
    if(!processed_generals){
        goto err;
    }

    for (i = 0; i < line_count; ++i){
        if(get_variables(generals[i], processed_generals) == 0){
            generals_dealloc(&processed_generals);
            goto err;
        }
    }

    return processed_generals;

    err:
        return NULL;
}

int get_variables(char *line, struct generals *generals){
    char *variable = NULL;
    size_t var_count, var_len;
    if (!line || !generals){
        error = POINTER_ERR;
        return 0;
    }

    var_len = 0;
    var_count = 0;

    variable = strtok(line, GENERALS_SPLITTER);
    while (variable != NULL){
        variable[strcspn(variable, "\n")] = 0;
        variable[strcspn(variable, "\r\n")] = 0;

        if(*variable == '\0'){
            variable = strtok(NULL, GENERALS_SPLITTER);
            continue;
        }
        
        var_len = strlen(variable) + 1;

        if (!check_variable_name(variable)){
            return 0;
        }

        if(var_len > MAX_VAR_LEN){
            error = VAR_NAME_ERR;
            return 0;
        }

        if(variables_alloc(generals, var_len, var_count, variable) == 0){
            return 0;
        }
        ++var_count;
        variable = strtok(NULL, GENERALS_SPLITTER);
    }

    if(generals_set_variables_count(generals, var_count) == 0){
        return 0;
    }

    return 1;
}

struct generals *generals_alloc(){
    struct generals *new_generals = NULL;

    new_generals = malloc(sizeof(*new_generals));

    if (!new_generals){
        error = MEMORY_ERR;
        return NULL;
    }

    if(generals_init(new_generals) == 0){
        free(new_generals);
        return NULL;
    }

    return new_generals;
}

int generals_init(struct generals *generals){
    if(!generals){
        error = POINTER_ERR;
        return 0;
    }

    generals->variables = NULL;
    generals->variables_count = 0;

    return 1;
}

void generals_deinit(struct generals *generals){
    if(!generals){
        return;
    }

    if (!generals->variables){
        goto end;
    }

    variables_dealloc(&(generals->variables), generals->variables_count);

    end:
    generals->variables_count = 0;
    return;
}

void generals_dealloc(struct generals **generals){
    if(!generals || !(*generals)){
        return;
    }

    generals_deinit(*generals);
    free(*generals);
    *generals = NULL;
    return;
}

char *variable_alloc(const size_t var_len, const char *var){
    char *new_var = NULL;
    if (!var || var_len == 0){
        error = POINTER_ERR;
        return NULL;
    }
    
    new_var = malloc(var_len);

    if(!new_var){
        error = MEMORY_ERR;
        return NULL;
    }

    if(variable_init(new_var, var, var_len) == 0){
        free(new_var);
        return NULL;
    }

    return new_var;
}

int variable_init(char *new_var, const char *var, const size_t var_len){
    if(!new_var || !var){
        error = POINTER_ERR;
        return 0;
    }

    strncpy(new_var, var, var_len);

    return 1;
}

int variables_alloc(struct generals *generals, const size_t var_len, const size_t var_count, const char *variable){
    char **new_var = NULL;
    size_t current_var_count;
    if (!generals || !variable || var_len == 0){
        error = POINTER_ERR;
        return 0;
    }

    current_var_count = var_count + generals->variables_count;
    
    new_var = realloc(generals->variables, (current_var_count + 1) * sizeof(char *));
    if(!new_var){
        error = MEMORY_ERR;
        return 0;
    }

    new_var[current_var_count] = variable_alloc(var_len, variable);

    if (!(new_var[current_var_count])){
        free(new_var);
        error = MEMORY_ERR;
        return 0;
    }

    if(generals_set_variables(generals, new_var) == 0){
        return 0;
    }

    return 1;
}

void variable_dealloc(char **variable){
    if(!variable || !(*variable)){
        return;
    }

    free(*variable);
    *variable = NULL;
    return;
    
}

void variables_dealloc(char ***variables, const size_t var_count){
    size_t i;
    if (!variables || !(*variables) || !(**variables) || var_count == 0){
        return;
    }

    for (i = 0; i < var_count; ++i){
        variable_dealloc(&((*variables)[i]));
    }
    
    free(*variables);
    *variables = NULL;
    return;
}

void print_generals(const struct generals *generals){
    size_t i;
    if(!generals){
        return;
    }

    for (i = 0; i < generals->variables_count; ++i){
        printf("Variable %lu name is: %s\n", i + 1, *(generals->variables + i));
    }

    return;
}

int generals_set_variables(struct generals *generals, char **variables){
    if(!generals || !variables){
        error = POINTER_ERR;
        return 0;
    }

    generals->variables = variables;

    return 1;
}

int generals_set_variables_count(struct generals *generals, const size_t var_count){
    if(!generals || var_count == 0){
        error = POINTER_ERR;
        return 0;
    }

    generals->variables_count += var_count;

    return 1;
}

int get_variable(const char *var, const struct generals *generals){
    size_t i;

    for (i = 0; i < generals->variables_count; ++i){
        if (strlen(var) != strlen(generals->variables[i])){
            continue;
        }
        
        if(!strcmp(var, generals->variables[i])){
            return i;
        }
    }

    return -1;
}

int search_variables(char *line, const struct generals *generals){
    size_t i;

    if (!line || !generals){
        return 0;
    }
    

    for (i = 0; i < generals->variables_count; ++i){
        if (strstr(line, generals->variables[i])){
            return 1;
        }
    }

    return 0;
}

int check_variable_name(char *variable_name){
    char *banned_chars = "{}()[]+-*/<>=^:,";

    if (!variable_name){
        error = POINTER_ERR;
        return 0;
    }

    if(strpbrk(variable_name, banned_chars)){
        error = SYNTAX_ERR;
        return 0;
    }

    if (isdigit(variable_name[0])){
        error = SYNTAX_ERR;
        return 0;
    }
    
    return 1;
}

int generals_cpy(struct generals *new_generals, struct generals *old_generals){
    size_t i, var_len;

    if(!new_generals || !old_generals){
        error = POINTER_ERR;
        return 0;
    }

    new_generals->variables = calloc(sizeof(char *), old_generals->variables_count);

    if (!new_generals->variables){
        error = MEMORY_ERR;
        return 0;
    }

    for (i = 0; i < old_generals->variables_count; ++i){
        var_len = strlen(old_generals->variables[i]) + 1;
        new_generals->variables[i] = calloc(sizeof(char), var_len);

        if(!new_generals){
            variables_dealloc(&new_generals->variables, old_generals->variables_count);
            error = MEMORY_ERR;
            return 0;
        }

        strncpy(new_generals->variables[i], old_generals->variables[i], var_len);
    }

    new_generals->variables_count = old_generals->variables_count;

    return 1;  
}