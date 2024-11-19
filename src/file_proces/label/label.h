#include"../../errors.h"

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef LABEL_H
#define LABEL_H

#define LINE_LENGTH 1024

#define GENERALS_SPLITTER " "

#define DELETE_COMMS(line)          \
    switch (delete_comments(line)){ \
        case 0:                     \
            return NULL;            \
            break;                  \
        case 2:                     \
            continue;               \
            break;                  \
        }                           

#define CHECK_LINE_LENGTH(line)                                                     \
    if(!strchr(line,'\n') && !strchr(line, EOF) && !strpbrk(line, "\r\n")){         \
            error = LINE_ERR;                                                       \
            return NULL;                                                            \
        }

typedef char label_type;

struct label{
    char *name;
    label_type label;
};

label_type identify_label(const char *line);

int delete_comments(char *line);

#endif