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

    if(!strncmp(line, "\n", 2) || !strncmp(line, "\r\n", 4) || *line == 0 || *line == EOF){
        return 2;
    }
    
    comm_position = strstr(line, "\\");

    if (comm_position){
        if(comm_position == line){
            return 2;
        }

        *comm_position = '\n';
        *(comm_position + 1) = '\0';
    }

    return 1;
}