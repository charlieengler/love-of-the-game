#ifndef UTILS_STRINGS_H
#define UTILS_STRINGS_H

int *csv_to_int_array(char *str, int *num_entries);
char **csv_to_string_array(char *str, int *num_entries);

char *int_array_to_csv(int *array, int num_entries);
char *string_array_to_csv(char **array, int num_entries);

int append_str(char **target, char *addition, int *current_size, int *allocated_size);

#endif // UTILS_STRINGS_H
