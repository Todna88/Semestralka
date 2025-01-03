#include"label.h"

const struct label LABELS[] = {
    {"Subject To", 1},
    {"Maximize", 2},
    {"Minimize", 3},
    {"Bounds", 4},
    {"Generals", 5},
    {"End", 6},
};

const size_t LABELS_COUNT = sizeof(LABELS) / sizeof(*LABELS);

label_type identify_label(const char *line){
    size_t i;
    if(!line){
        error = POINTER_ERR;
        return 7;
    }

    for (i = 0; i < LABELS_COUNT; ++i) {
        if (strstr(line, LABELS[i].name)) {
            return LABELS[i].label;
        }
    }

    return 0;
}

int delete_comments(char *line){
    char *comm_position = NULL;

    if (!line){
        error = POINTER_ERR;
        return 0;
    }

    if(!strncmp(line, NEWLINE, NEWLINE_LEN) || !strncmp(line, NEWLINE_WINDOWS, NEWLINE_WINDOWS_LEN) || *line == 0 || *line == EOF){
        return 2;
    }
    
    comm_position = strstr(line, COMMENT);

    if (comm_position){
        if(comm_position == line){
            return 2;
        }

        *comm_position = NEWLINE_CHAR;
        *(comm_position + END_CHAR_LEN) = ENDING_CHAR;
    }

    return 1;
}