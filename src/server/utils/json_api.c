#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/server/utils/json_api.h"
#include "../../include/server/utils/json_parser.h"

#include "../../include/server/utils/numbers.h"

int append_str(char **target, char *addition, long *current_size, long *allocated_size) {
    long addition_size = strlen(addition);
    long difference = *allocated_size - *current_size;

    if (addition_size > difference - 1) {
        *allocated_size += difference * 1.5;

        char *new_str = (char *)malloc(*allocated_size * sizeof(char));
        strcpy(new_str, *target);

        free(*target);

        *target = new_str;
    }

    strcat(*target, addition);

    *current_size += addition_size;

    ++(*current_size);

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

char *json_object_to_string(struct json_object *json_obj) {
    long total_length = 0;
    long alloc_size = 100;
    char *str = (char *)malloc(alloc_size * sizeof(char));
    strcpy(str, "{");
    ++total_length;
    str[total_length] = '\0';
    ++total_length;

    if (json_obj->num_entries == 0) {
        strcat(str, "}");

        str[2] = '\0';

        return str;
    }

    char *key = json_obj->keys[0];

    append_str(&str, "\"", &total_length, &alloc_size);
    append_str(&str, key, &total_length, &alloc_size);
    append_str(&str, "\":", &total_length, &alloc_size);

    // TODO: Uncomment me when implemented
    // struct json_value *json_val = json_object_get_value(json_obj, key);

    struct json_value *json_val = json_obj->values[0];

    char *output = json_value_to_string(json_val);

    if (!output) {
        // TODO: Error message with reason for failure
        goto fail;
    }

    append_str(&str, output, &total_length, &alloc_size);

    // TODO: Figure out why this causes memory errors
    // free(output);

    for (unsigned long i = 1; i < json_obj->num_entries; ++i) {
        char *key = json_obj->keys[i];

        append_str(&str, ",\"", &total_length, &alloc_size);
        append_str(&str, key, &total_length, &alloc_size);
        append_str(&str, "\":", &total_length, &alloc_size);

        // TODO: Uncomment me when implemented
        // struct json_value *json_val = json_object_get_value(json_obj, key);

        struct json_value *json_val = json_obj->values[i];

        output = json_value_to_string(json_val);

        if (!output) {
            // TODO: Error message with reason for failure
            goto fail;
        }

        append_str(&str, output, &total_length, &alloc_size);

        // TODO: Figure out why this causes memory errors
        // free(output);
    }

    append_str(&str, "}", &total_length, &alloc_size);

    return str;

fail:
    free(str);
    return NULL;
}

char *json_array_to_string(struct json_array *json_arr) {
    long total_length = 0;
    long alloc_size = 100;
    char *str = (char *)malloc(alloc_size * sizeof(char));
    strcpy(str, "[");
    ++total_length;

    if (json_arr->length == 0) {
        strcat(str, "]");

        str[2] = '\0';

        return str;
    }

    struct json_value *json_val = json_arr->values[0];

    char *output = json_value_to_string(json_val);

    if (!output) {
        // TODO: Error message with reason for failure
        goto fail;
    }

    append_str(&str, output, &total_length, &alloc_size);

    free(output);

    for (unsigned long i = 1; i < json_arr->length; ++i) {
        struct json_value *json_val = json_arr->values[i];

        output = json_value_to_string(json_val);

        if (!output) {
            // TODO: Error message with reason for failure
            goto fail;
        }

        append_str(&str, output, &total_length, &alloc_size);

        free(output);
    }

    append_str(&str, "}", &total_length, &alloc_size);

    return str;

fail:
    free(str);
    return str;
}

char *json_true_to_string() {
    char *str = (char *)malloc((strlen("true") + 1) * sizeof(char));

    strcpy(str, "true");

    str[strlen("true")] = '\0';

    return str;
}

char *json_false_to_string() {
    char *str = (char *)malloc((strlen("false") + 1) * sizeof(char));

    strcpy(str, "false");

    str[strlen("false")] = '\0';

    return str;
}

char *json_null_to_string() {
    char *str = (char *)malloc((strlen("null") + 1) * sizeof(char));

    strcpy(str, "null");

    str[strlen("null")] = '\0';

    return str;
}

char *json_value_to_string(struct json_value *json_val) {
    long total_length = 0;
    long alloc_size = 100;
    char *str = (char *)malloc(alloc_size * sizeof(char *));

    // TODO: Destroy the JSON related structs as they are added to the string, or destroy the whole value at the end of the function call
    char *output = NULL;
    switch (json_val->type) {
    case JSON_STRING:
        output = json_string_to_string((char *)json_val->data);
        break;

    case JSON_NUMBER:
        output = json_number_to_string((struct json_number *)json_val->data);
        break;

    case JSON_OBJECT:
        output = json_object_to_string((struct json_object *)json_val->data);
        break;

    case JSON_ARRAY:
        output = json_object_to_string((struct json_object *)json_val->data);
        break;

    case JSON_TRUE:
        output = json_true_to_string();
        break;

    case JSON_FALSE:
        output = json_false_to_string();
        break;

    case JSON_NULL:
        output = json_null_to_string();
        break;

    case JSON_UNDEFINED:
    default:
        // TODO: Error message with reason for failure
        goto fail;
    }

    if (!output) {
        // TODO: Error message with reason for failure
        goto fail;
    }

    // TODO: Check for errors
    append_str(&str, output, &total_length, &alloc_size);

    free(output);

    return str;

fail:
    free(str);
    return NULL;
}
