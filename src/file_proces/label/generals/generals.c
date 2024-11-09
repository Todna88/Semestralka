#include "generals.h"

struct generals *process_generals(FILE *stream){
    label_type label;
    struct generals *generals;
    char line[LINE_LENGTH];
        while (fgets(line, sizeof(line), stream)) {
            CHECHK_LINE_LENGTH(line);

            DELETE_COMMS(line);

            label = identify_label(line);
            if(label){
                goto end;
            }

            generals = get_variables(line);
        }

    end:
        fseek(stream, -strlen(line), SEEK_CUR);
        return generals;
}

struct generals *get_variables(char *line){
    char *variable;
    char **variables;
    size_t vars_count, i;
    struct generals *generals;
    if (!line){
        return NULL;
    }

    vars_count = var_count(line);

    if (vars_count == 0){
        return NULL;
    }

    variables = variables_alloc(vars_count * sizeof(variable));
    variable = line;
    for (i = 0; i < vars_count; ++i){
        variable[strcspn(line, "\n")] = 0;
        variables[i] = variable_alloc(strlen(variable)+1);
        if(!variables[i]){
            return NULL;
        }
        strcpy(variables[i], variable);
        variable += strlen(variable) + 1;
    }

    generals = generals_alloc(variables, vars_count);
    if (!generals){
        return NULL;
    }
    
    return generals;
}

struct generals *generals_alloc(char **variables, size_t var_count){
    struct generals *new_generals;
    if(!variables || var_count == 0){
        return NULL;
    }

    new_generals = malloc(sizeof(*new_generals));
    if (!new_generals){
        return NULL;
    }

    if(!generals_init(new_generals, variables ,var_count)){
        free(new_generals);
        return NULL;
    }

    return new_generals;
}

int generals_init(struct generals *generals, char **variables, size_t var_count){
    if(!generals || !variables || var_count == 0){
        return 0;
    }

    generals->variables = variables;
    generals->variables_count = var_count;

    return 1;
}

void generals_deinit(struct generals *generals){
    if(!generals){
        return;
    }

    if (!generals->variables){
        return;
    }

    variables_dealloc(&(generals->variables), generals->variables_count);
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

size_t var_count(char *line){
    size_t var_count;
    char * var;
    if (!line){
        return 0;
    }

    var = strtok(line, GENERALS_SPLITTER);
    var_count = 0;
    while(var != NULL) {
        ++var_count;
        var = strtok(NULL, GENERALS_SPLITTER);
   }

   return var_count;
}

char *variable_alloc(size_t size){
    char *new_var;
    if (size == 0){
        return NULL;
    }
    
    new_var = malloc(size);
    if(!new_var){
        return NULL;
    }

    return new_var;
}

char **variables_alloc(size_t size){
    char **new_var;
    if (size == 0){
        return NULL;
    }
    
    new_var = malloc(size);
    if(!new_var){
        return NULL;
    }

    return new_var;
}

void variable_dealloc(char **variable){
    if(!variable || !(*variable)){
        return;
    }

    free(*variable);
    *variable = NULL;
    return;
    
}

void variables_dealloc(char ***variables, size_t var_size){
    size_t i;
    if (!variables || !(*variables) || !(**variables) || var_size == 0){
        return;
    }

    for (i = 0; i < var_size; ++i){
        variable_dealloc(&(*(*variables + i)));
    }
    
    free(*variables);
    *variables = NULL;
    return;
}

void print_generals(struct generals *generals){
    size_t i;
    if(!generals){
        return;
    }

    for (i = 0; i < generals->variables_count; ++i){
        printf("Variable %lu name is: %s\n", i + 1, *(generals->variables + i));
    }

    return;
}