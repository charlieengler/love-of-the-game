#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/server/utils/json_api.h"
#include "../../include/server/utils/json_parser.h"

#include "../../include/server/utils/numbers.h"

int append_str(char **target, char *addition, long *current_size, long *allocated_size) {
    char *output = *target;

    long addition_size = strlen(addition);
    long difference = *allocated_size - *current_size;

    if (addition_size > difference - 1) {
        *allocated_size += difference * 1.5;

        output = (char *)malloc(*allocated_size * sizeof(char));
        strcpy(output, *target);
    }

    strcat(output, addition);

    *current_size += difference;

    output[*current_size] = '\0';

    ++(*current_size);

    *target = output;

    // TODO: Error codes for various failures
    return 0;
}

char *json_string_to_string(char *json_str) {
    // TODO: Escape characters that need to be
    char *output = (char *)malloc((strlen(json_str) + 3) * sizeof(char));
    strcpy(output, "\"");
    strcat(output, json_str);
    strcat(output, "\"");

    output[strlen(json_str) + 2] = '\0';

    return output;
}

char *json_number_to_string(struct json_number *json_num) {
    // TODO: Support long long data type, not just int via int_num_places

    int int_places = int_num_places(json_num->integer);
    int fraction_places = int_num_places(json_num->fraction);
    int exponent_places = int_num_places(json_num->exponent);

    char *output = (char *)malloc((int_places + fraction_places + exponent_places + 1) * sizeof(char));

    switch (json_num->type) {
    case JSON_INTEGER:
        sprintf(output, "%lld", json_num->integer);

        output[int_places] = '\0';
        break;

    case JSON_FRACTION:
        sprintf(output, "%lld.%lld", json_num->integer, json_num->fraction);

        output[int_places + fraction_places] = '\0';
        break;

    case JSON_EXPONENTIAL:
        sprintf(output, "%lld.%llde%lld", json_num->integer, json_num->fraction, json_num->exponent);

        output[int_places + fraction_places + exponent_places] = '\0';
        break;

    case JSON_UNDEFINED_NUMBER:
    default:
        goto fail;
    }

    return output;

fail:
    free(output);
    return NULL;
}

// TODO: Objects, arrays, true, false, and null to string

char *json_value_to_string(struct json_value *json_val) {
    long total_length = 0;
    long alloc_size = 100;
    char *str = (char *)malloc(alloc_size * sizeof(char *));

    switch (json_val->type) {
    case JSON_STRING:
        char *output = json_string_to_string((char *)json_val->data);

        if (!output) {
            // TODO: Error message with reason for failure
            goto fail;
        }

        // TODO: Check for errors
        append_str(&str, output, &total_length, &alloc_size);

        free(output);
        break;

    case JSON_NUMBER:
        break;

    case JSON_OBJECT:
        break;

    case JSON_ARRAY:
        break;

    case JSON_TRUE:
        break;

    case JSON_FALSE:
        break;

    case JSON_NULL:
        break;

    case JSON_UNDEFINED:
    default:
        // TODO: Error message with reason for failure
        goto fail;
    }

    return str;

fail:
    free(str);
    return NULL;
}
