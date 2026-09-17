#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/server/utils/numbers.h"
#include "../../include/server/utils/strings.h"

// TODO: Error handling
int *csv_to_int_array(char *str, int *num_entries) {
    int alloc_num = 10;

    int *array = (int *)malloc(alloc_num * sizeof(int));

    *num_entries = 0;

    char *str_copy = (char *)malloc(strlen(str) * sizeof(char));
    char *start = str_copy;
    strcpy(str_copy, str);
    char *current_index_string = str_copy;
    while (*str_copy) {
        switch (*str_copy) {
        case ',':
            *str_copy = '\0';

            if (*num_entries >= alloc_num) {
                alloc_num *= 1.5;

                int *new_array = (int *)malloc(alloc_num * sizeof(int));
                memcpy(new_array, array, *num_entries * sizeof(int));
                free(array);
                array = new_array;
            }

            array[*num_entries] = atoi(current_index_string);

            current_index_string = str_copy + 1;

            (*num_entries)++;
            break;
        default:
            break;
        }

        ++str_copy;
    }

    array[*num_entries] = atoi(current_index_string);

    (*num_entries)++;

    free(start);

    return array;
}

// TODO: Error handling
char **csv_to_string_array(char *str, int *num_entries) {
    int alloc_num = 10;

    char **array = (char **)malloc(alloc_num * sizeof(char *));

    *num_entries = 0;

    char *str_copy = (char *)malloc(strlen(str) * sizeof(char));
    char *start = str_copy;
    strcpy(str_copy, str);
    char *current_index_string = str_copy;
    while (*str_copy) {
        switch (*str_copy) {
        case ',':
            *str_copy = '\0';

            if (*num_entries >= alloc_num) {
                alloc_num *= 1.5;

                char **new_array = (char **)malloc(alloc_num * sizeof(char *));
                memcpy(new_array, array, *num_entries * sizeof(char *));
                free(array);
                array = new_array;
            }

            char *new_entry = (char *)malloc((strlen(current_index_string) + 1) * sizeof(char));
            strcpy(new_entry, current_index_string);
            new_entry[strlen(current_index_string)] = '\0';
            array[*num_entries] = new_entry;

            current_index_string = str_copy + 1;

            (*num_entries)++;
            break;
        default:
            break;
        }

        ++str_copy;
    }

    char *new_entry = (char *)malloc((strlen(current_index_string) + 1) * sizeof(char));
    strcpy(new_entry, current_index_string);
    new_entry[strlen(current_index_string)] = '\0';
    array[*num_entries] = new_entry;

    (*num_entries)++;

    free(start);

    return array;
}

char *int_array_to_csv(int *array, int num_entries) {
    char **parsed_entries = (char **)malloc(num_entries * sizeof(char *));

    int total_size = 0;

    int num_places = int_num_places(array[0]);

    parsed_entries[0] = (char *)malloc((num_places + 1) * sizeof(char));
    sprintf(parsed_entries[0], "%d", array[0]);

    total_size += num_places;

    for (int i = 1; i < num_entries; ++i) {
        int num_places = int_num_places(array[i]);

        parsed_entries[i] = (char *)malloc((num_places + 2) * sizeof(char));
        sprintf(parsed_entries[i], ",%d", array[i]);

        total_size += num_places + 1;
    }

    ++total_size;

    char *csv = (char *)malloc(total_size * sizeof(char));

    strcpy(csv, parsed_entries[0]);

    for (int i = 1; i < num_entries; ++i) {
        strcat(csv, parsed_entries[i]);

        free(parsed_entries[i]);
    }

    csv[total_size - 1] = '\0';

    free(parsed_entries);

    return csv;
}
