#include "file_proces/file_proces.h"
#include "flags/flags.h"

#include <stdlib.h>
#include <stdio.h>

error_type error = 1;

int main(int argc, char *argv[]){
    FILE *file;
    int i;
    /*char *output_file;*/
    char *input_file;

    /*output_file = NULL;*/
    input_file = NULL;

    if (argc < 2){
        puts("Input file not found!\n");
        error = 1;
        return error;
    }
    
    if (argc > 6){
        puts("Too many arguments!\n");
        error = 6;
        return error;
    }
    
    for (i = 1; i < argc; ++i){
        if (is_flag(argv[i])){

            if ((i) == (argc - 1)){
                puts("Usage: lp.exe <input-file> -o <output-file-path> --output <output-file-path>\n");
                error = FLAG_ERR;
                return error;
            }
            
            /*output_file = argv[i+1];*/
            i += 1;
            continue;
        }

        input_file = argv[i];
        
    }
    
    
    file = fopen(input_file, "r");

    if (!file){
        puts("Input file not found!\n");
        error = 1;
        return error;
    }

    if(process_file(file) == 0){
        fclose(file);
        return error;
    }

    fclose(file);
    return EXIT_SUCCESS;
    
}