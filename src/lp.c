#include "file_proces/file_proces.h"
#include "flags/flags.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){
    FILE *file;
    int i;
    char *output_file;
    char *input_file;

    output_file = NULL;
    input_file = NULL;

    if (argc < 2){
        puts("Input file not found!\n");
        return EXIT_FAILURE;
    }
    
    if (argc > 6){
        puts("Too many arguments!\n");
        return EXIT_FAILURE;
    }
    
    for (i = 1; i < argc; ++i){
        if (is_flag(argv[i])){

            if ((i) == (argc - 1)){
                puts("Usage: lp.exe <input-file> -o <path> --output <path>\n");
                return EXIT_FAILURE;
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
        return EXIT_FAILURE;
    }

    process_file(file);
    fclose(file);
    return EXIT_SUCCESS;
    
}