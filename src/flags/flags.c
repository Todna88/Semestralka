#include"flags.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const flags_type flags = {"-o", "--output"};

int is_flag(char *flag){

    if (!flag){
        return 0;
    }
    
    if (!strcmp(flags.o, flag)){
        return 1;
    }

    else if (!strcmp(flags.output, flag)){
        return 1;
    }
    
    return 0;
}