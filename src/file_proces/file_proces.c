#include"file_proces.h"

int process_file(FILE *file){
    struct generals *generals;
    label_type label;
    char line[LINE_LENGTH];

    while (fgets(line, sizeof(line), file)) {
        CHECHK_LINE_LENGTH(line);

        DELETE_COMMS(line);

        label = identify_label(line);

        switch (label)
        {
        case 1:
            printf("Processing Subject to\n");
            /*process_subject_to(file);*/
            continue;
            break;
        
        case 2:
            printf("Processing Maximize\n");
            /*process_maximize(file);*/
            continue;
            break;
        
        case 3:
            printf("Processing Minimize\n");
           /*process_minimize(file);*/
            continue;
            break;

        case 4:
            printf("Processing bounds\n");
            /*process_bounds(file);*/
            continue;
            break;

        case 5:
            printf("Processing Generals\n");
            generals = process_generals(file);
            break;

        case 6:
            goto end;
            break;
        }

        printf("%s", line);
        }
    end:
    print_generals(generals);
    generals_dealloc(&generals);
    return 1;
}