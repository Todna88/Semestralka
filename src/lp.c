#include "file_proces/file_proces.h"
#include "simplex/simplex.h"
#include "flags/flags.h"

#include <stdlib.h>
#include <stdio.h>

error_type error = 0;

int main(int argc, char *argv[]){
    struct processed_file *processed_file = NULL;
    char *input_file = NULL;
    char *output_file = NULL;
    FILE *file;
    int i;

    if (argc < 2){
        puts("Input file not found!\n");
        error = INPUT_FILE_ERR;
        return error;
    }
    
    if (argc > 6){
        puts("Too many arguments!\n");
        error = ARGUMENT_ERR;
        return error;
    }
    
    for (i = 1; i < argc; ++i){
        if (is_flag(argv[i])){

            if ((i) == (argc - 1)){
                puts("Usage: lp.exe <input-file> -o <output-file-path> --output <output-file-path>\n");
                error = FLAG_ERR;
                return error;
            }
            
            output_file = argv[i+1];
            i += 1;
            continue;
        }

        input_file = argv[i];
        
    }
    
    
    file = fopen(input_file, "r");

    if (!file){
        puts("Input file not found!\n");
        error = INPUT_FILE_ERR;
        return error;
    }

    processed_file = process_file(file);

    if(!processed_file){
        fclose(file);
        goto err;
    }

    fclose(file);

    if(simplex(processed_file, output_file)){
        processed_file_dealloc(&processed_file);
        return EXIT_SUCCESS;
    }

    processed_file_dealloc(&processed_file);

    err:
    switch (error){
        case OUTPUT_FILE_ERR:
            printf("Invalid output destination!\n");
            return error;
            break;

        case VARIABLE_ERR:
            return error;
            break;

        case SYNTAX_ERR:
            printf("Syntax error!\n");
            return error;
            break;

        case FUNC_BOUND_ERR:
            printf("Objective function is unbounded.\n");
            return error;
            break;

        case SOLUTION_ERR:
            printf("No feasible solution exists.\n");
            return error;
            break;

        case MEMORY_ERR:
            printf("Shortage of memory!\n");
            return error;
            break;
        
        case LINE_ERR:
            printf("Too long line!\n");
            return error;
            break;

        case VAR_NAME_ERR:
            printf("Too long variable name!\n");
            return error;
            break;

        case POINTER_ERR:
            printf("Error of a function argument!\n");
            return error;
            break;
    }

    return error;
}