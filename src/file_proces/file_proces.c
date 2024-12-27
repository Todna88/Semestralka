#include"file_proces.h"

int process_file(FILE *file){
    struct divided_file *divided_file = NULL;
    struct generals *generals = NULL;
    struct function *function = NULL;
    divided_file = divide_file(file);

    if (!divided_file){
        goto err;
    }

    generals = process_generals(divided_file->generals, divided_file->generals_line_count);

    if(!generals){
        goto err;
    }

    function = process_function(divided_file->function, generals);

    if(!function){
        goto err;
    }

    print_generals(generals);
    print_coefs(function, generals);

    generals_dealloc(&generals);
    function_dealloc(&function);
    divided_file_dealloc(&divided_file);

    return 1;

    err:
        generals_dealloc(&generals);
        function_dealloc(&function);
        divided_file_dealloc(&divided_file);
        return 0;
}

struct divided_file *divide_file(FILE *file){
    struct divided_file *divided_file = NULL; 
    size_t label_line_len;
    size_t line_count;
    label_type label;
    char line[LINE_LENGTH];

    label = SYNTAX_ERR;

    divided_file = divided_file_alloc();
    if (!divided_file){
        error = MEMORY_ERR;
        return NULL;
    }
    
    while (fgets(line, sizeof(line), file)) {
        CHECK_LINE_LENGTH(line)

        DELETE_COMMS(line)

        label_line_len = strlen(line) + 1;

        switch (identify_label(line))
        {
        case 0:
            break;

        case 6:
            label = identify_label(line);
            break;

        case 7:
            goto err;
            break;
        
        default:
            label = identify_label(line);
            line_count = 0;
            continue;
            break;
        }
        

        switch (label)
        {
        case 1:
            ++line_count;
            if(subject_to_alloc(divided_file, label_line_len, line_count, line) == 0){
                goto err;
            }
            continue;
            break;
        
        case 2:
            ++line_count;

            if(line_count > 1){
                error = SYNTAX_ERR;
                goto err;
            }

            if(func_alloc(divided_file, label_line_len, MAX_TYPE, line) == 0){
                goto err;
            }
            continue;
            break;
        
        case 3:
            ++line_count;

            if(line_count > 1){
                error = SYNTAX_ERR;
                goto err;
            }

            if(func_alloc(divided_file, label_line_len, MIN_TYPE, line) == 0){
                goto err;
            }
            continue;
            break;

        case 4:
            ++line_count;
            if(bounds_alloc(divided_file, label_line_len, line_count, line) == 0){
                goto err;
            }
            continue;
            break;

        case 5:
            ++line_count;
            if(gen_alloc(divided_file, label_line_len, line_count, line) == 0){
                goto err;
            }
            continue;
            break;

        case 6:
            goto end;
            break;
        }
    }

    error = SYNTAX_ERR;
    err:
    divided_file_dealloc(&divided_file);
    return NULL;

    end:
    return divided_file;
}

struct divided_file *divided_file_alloc(){
    struct divided_file *new_divided_file = NULL;

    new_divided_file = malloc(sizeof(*new_divided_file));

    if (!new_divided_file){
        error = MEMORY_ERR;
        return NULL;
    }

    if(divided_file_init(new_divided_file) == 0){
        free(new_divided_file);
        return NULL;
    }

    return new_divided_file;
}

int divided_file_init(struct divided_file *divided_file){
    if (!divided_file){
        error = POINTER_ERR;
        return 0;
    }
    
    divided_file->bounds = NULL;
    divided_file->bounds_line_count = 0;
    divided_file->function = NULL;
    divided_file->function_type = NULL;
    divided_file->generals = NULL;
    divided_file->generals_line_count = 0;
    divided_file->subject_to = NULL;
    divided_file->subject_to_line_count = 0;

    return 1;
}

int subject_to_alloc(struct divided_file *file, const size_t line_len, const size_t line_count, const char *line){
    char **new_lines = NULL;
    if (!file || !line || line_len == 0 || line_count == 0){
        error = POINTER_ERR;
        return 0;
    }

    new_lines = realloc(file->subject_to, line_count * sizeof(char *));

    if (!new_lines){
        error = MEMORY_ERR;
        return 0;
    }

    new_lines[line_count - 1] = line_alloc(line_len, line);

    if (!(new_lines[line_count - 1])){
        free(new_lines);
        return 0;
    }

    file->subject_to = new_lines;
    file->subject_to_line_count = line_count;

    return 1;
}

int func_alloc(struct divided_file *file, const size_t line_len, char *function_type, const char *line){
    char *new_line = NULL;
    if (!file || !function_type || !line || line_len == 0){
        error = POINTER_ERR;
        return 0;
    }

    new_line = line_alloc(line_len, line);

    if (!new_line){
        return 0;
    }

    file->function = new_line;
    file->function_type = function_type;

    return 1;
}

int bounds_alloc(struct divided_file *file, const size_t line_len, const size_t line_count, const char *line){
    char **new_lines = NULL;
    if (!file || !line || line_len == 0 || line_count == 0){
        error = POINTER_ERR;
        return 0;
    }

    new_lines = realloc(file->bounds, line_count * sizeof(char *));

    if (!new_lines){
        error = MEMORY_ERR;
        return 0;
    }

    new_lines[line_count - 1] = line_alloc(line_len, line);

    if (!(new_lines[line_count - 1])){
        return 0;
    }

    file->bounds = new_lines;
    file->bounds_line_count = line_count;

    return 1;

}

int gen_alloc(struct divided_file *file, const size_t line_len, const size_t line_count, const char *line){
    char **new_lines = NULL;
    if (!file || !line || line_len == 0 || line_count == 0){
        error = POINTER_ERR;
        return 0;
    }

    new_lines = realloc(file->generals, line_count * sizeof(char *));

    if (!new_lines){
        error = MEMORY_ERR;
        return 0;
    }

    new_lines[line_count - 1] = line_alloc(line_len, line);

    if (!(new_lines[line_count - 1])){
        free(new_lines);
        return 0;
    }

    file->generals = new_lines;
    file->generals_line_count = line_count;

    return 1;

}

char *line_alloc(const size_t line_len, const char *line){
    char *new_line = NULL;
    if(!line || line_len == 0){
        error = POINTER_ERR;
        return NULL;
    }

    new_line = malloc(line_len);

    if(!new_line){
        error = MEMORY_ERR;
        return NULL;
    }

    if(!line_init(new_line, line, line_len)){
        free(new_line);
        return NULL;
    }

    return new_line;
}

int line_init(char *new_line, const char *line, const size_t line_len){
    if(!new_line || !line || line_len == 0){
        error = POINTER_ERR;
        return 0;
    }

    strncpy(new_line, line, line_len);
    return 1;
}

void line_dealloc(char **line){
    if(!line || !(*line)){
        return;
    }

    free(*line);

    *line = NULL;
    return;
}

void divided_file_dealloc(struct divided_file **devided_file){
    if(!devided_file || !(*devided_file)){
        return;
    }

    divided_file_deinit(*devided_file);

    free(*devided_file);
    *devided_file = NULL;

    return;
}

void divided_file_deinit(struct divided_file *devided_file){
    if(!devided_file){
        return;
    }

    label_dealloc(&(devided_file->bounds), devided_file->bounds_line_count);
    devided_file->bounds_line_count = 0;

    label_dealloc(&(devided_file->subject_to), devided_file->subject_to_line_count);
    devided_file->subject_to_line_count = 0;

    label_dealloc(&(devided_file->generals), devided_file->generals_line_count);
    devided_file->generals_line_count = 0;

    line_dealloc(&(devided_file->function));
    devided_file->function_type = NULL;
}

void label_dealloc(char ***label, const size_t line_count){
    size_t i;
    if(!label || !(*label) || !(**label) || line_count == 0){
        return;
    }

    for (i = 0; i < line_count; ++i){
        line_dealloc(&((*label)[i]));
    }

    free(*label);
    *label = NULL;

    return;
}