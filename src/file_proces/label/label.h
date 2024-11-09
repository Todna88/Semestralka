#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#ifndef LABEL_H
#define LABEL_H

#define LINE_LENGTH 1024

#define GENERALS_SPLITTER " "

#define DELETE_COMMS(line)          \
    switch (delete_comments(line))  \
        {                           \
        case 0:                     \
            return NULL;               \
            break;                  \
        case 2:                     \
            continue;               \
            break;                  \
        }

#define CHECHK_LINE_LENGTH(line)                                        \
    if(!strchr(line,'\n')){                                             \
            printf("Řádka je delší, než dokáže program zpracovat!\n");  \
            return NULL;                                                   \
        }

typedef char label_type;

struct label{
    char *name;
    label_type label;
};

label_type identify_label(const char *line);

int delete_comments(char *line);

#endif