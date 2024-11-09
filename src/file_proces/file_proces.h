#include"label/generals/generals.h"

#ifndef FILE_PROCES_H
#define FILE_PROCES_H

struct processed_file{
    char **variables;
    double *func_coef;
    double ** constran_coef;
    double *bounds;
};

int process_file(FILE *file);

void process_subject_to(FILE *stream);

void process_maximize(FILE *stream);

void process_minimize(FILE *stream);

void process_bounds(FILE *stream);

#endif