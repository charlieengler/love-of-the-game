#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../include/server/utils/json_api.h"

#include "../../../include/server/utils/numbers.h"
#include "../../../include/server/utils/strings.h"

#include "internal.h"

char *json_string_to_string(char *json_str, int depth) {
    // TODO: Escape characters that need to be
    char *output = (char *)calloc((strlen(json_str) + 3), sizeof(char));
    strcpy(output, "\"");
    strcat(output, json_str);
    strcat(output, "\"");

    output[strlen(json_str) + 2] = '\0';

    return output;
}

char *json_number_to_string(struct json_number *json_num, int depth) {
    // TODO: Support long long data type, not just int via int_num_places

    int int_places = int_num_places(json_num->integer);
    int fraction_places = int_num_places(json_num->fraction);
    int exponent_places = int_num_places(json_num->exponent);

    char *output = (char *)calloc((int_places + fraction_places + exponent_places + 3), sizeof(char));

    switch (json_num->type) {
    case JSON_INTEGER:
        sprintf(output, "%lld", json_num->integer);

        output[int_places] = '\0';
        break;

    case JSON_FRACTION:
        sprintf(output, "%lld.%lld", json_num->integer, json_num->fraction);

        output[int_places + fraction_places + 1] = '\0';
        break;

    case JSON_EXPONENTIAL:
        sprintf(output, "%lld.%llde%lld", json_num->integer, json_num->fraction, json_num->exponent);

        output[int_places + fraction_places + exponent_places + 2] = '\0';
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

char *json_object_to_string(struct json_object *json_obj, int depth) {
    int total_length = 0;
    int alloc_size = 100;
    char *str = (char *)calloc(alloc_size, sizeof(char));
    append_str(&str, "{", &total_length, &alloc_size);

    if (json_obj->num_entries == 0) {
        goto out;
    }

    for (int i = 0; i < json_obj->num_entries; ++i) {
        append_str(&str, "\n", &total_length, &alloc_size);

        for (int j = 0; j < depth + 1; ++j) {
            append_str(&str, "    ", &total_length, &alloc_size);
        }

        char *key = json_obj->keys[i];

        append_str(&str, "\"", &total_length, &alloc_size);
        append_str(&str, key, &total_length, &alloc_size);
        append_str(&str, "\": ", &total_length, &alloc_size);

        // TODO: Error checking
        struct json_value *json_val = json_object_get_value(json_obj, key);

        char *output = json_value_to_string_helper(json_val, depth + 1);

        if (!output) {
            // TODO: Error message with reason for failure
            goto fail;
        }

        append_str(&str, output, &total_length, &alloc_size);

        if (i + 1 < json_obj->num_entries) {
            append_str(&str, ",", &total_length, &alloc_size);
        }

        free(output);
    }

    append_str(&str, "\n", &total_length, &alloc_size);

    for (int j = 0; j < depth; ++j) {
        append_str(&str, "    ", &total_length, &alloc_size);
    }

out:
    append_str(&str, "}", &total_length, &alloc_size);

    return str;

fail:
    free(str);
    return NULL;
}

char *json_array_to_string(struct json_array *json_arr, int depth) {
    int total_length = 0;
    int alloc_size = 100;
    char *str = (char *)calloc(alloc_size, sizeof(char));
    append_str(&str, "[", &total_length, &alloc_size);

    if (json_arr->length == 0) {
        goto out;
    }

    for (int i = 0; i < json_arr->length; ++i) {
        append_str(&str, "\n", &total_length, &alloc_size);

        for (int j = 0; j < depth + 1; ++j) {
            append_str(&str, "    ", &total_length, &alloc_size);
        }

        struct json_value *json_val = json_arr->values[i];

        char *output = json_value_to_string_helper(json_val, depth + 1);

        if (!output) {
            // TODO: Error message with reason for failure
            goto fail;
        }

        append_str(&str, output, &total_length, &alloc_size);

        if (i + 1 < json_arr->length) {
            append_str(&str, ",", &total_length, &alloc_size);
        }

        free(output);
    }

    append_str(&str, "\n", &total_length, &alloc_size);

    for (int j = 0; j < depth; ++j) {
        append_str(&str, "    ", &total_length, &alloc_size);
    }

out:
    append_str(&str, "]", &total_length, &alloc_size);

    return str;

fail:
    free(str);
    return NULL;
}

char *json_true_to_string(int depth) {
    char *str = (char *)calloc((strlen("true") + 1), sizeof(char));

    strcpy(str, "true");

    str[strlen("true")] = '\0';

    return str;
}

char *json_false_to_string(int depth) {
    char *str = (char *)calloc((strlen("false") + 1), sizeof(char));

    strcpy(str, "false");

    str[strlen("false")] = '\0';

    return str;
}

char *json_null_to_string(int depth) {
    char *str = (char *)calloc((strlen("null") + 1), sizeof(char));

    strcpy(str, "null");

    str[strlen("null")] = '\0';

    return str;
}
