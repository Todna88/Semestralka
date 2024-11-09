#include <stdio.h>
#ifndef FLAGS_H
#define FLAGS_H

/*const char *labels[]= {"Subject To", "Maximize", "Minimize", "Bounds", "Generals", "End"};*/

typedef struct flags{
    char *o;
    char *output;
} flags_type;

int is_flag(char *flag);
#endif