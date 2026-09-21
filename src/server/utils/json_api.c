#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/server/utils/json_api.h"
#include "../../include/server/utils/json_parser.h"

#include "../../include/server/utils/numbers.h"

int append_str(char **target, char *addition, int *current_size, int *allocated_size) {
    int addition_size = strlen(addition);
    int difference = *allocated_size - *current_size;

    if (addition_size > difference - 1) {
        *allocated_size += difference * 1.5;

        char *new_str = (char *)calloc(*allocated_size, sizeof(char));
        strcpy(new_str, *target);

        free(*target);

        *target = new_str;
    }

    // TODO: If the string is supposed to be empty (current_size == 0), then this relies on nothing being resident in memory
    //       before appending. This means that reused memory after free is called causes issues with the memory being full
    //       of garbage if using malloc alone when creating the new strings. Therefore, calloc is used, but this is not as
    //       performant. It may be worth finding a happy medium, maybe by only zeroing out the first byte of malloc'd memory,
    //       or maybe modifying this function to zero out the first byte automatically if current_size == 0. Neither seems
    //       particularly clean
    strcat(*target, addition);

    *current_size += addition_size;

    // TODO: Error codes for various failures
    return 0;
}

char *json_string_to_string(char *json_str) {
    // TODO: Escape characters that need to be
    char *output = (char *)calloc((strlen(json_str) + 3), sizeof(char));
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

char *json_object_to_string(struct json_object *json_obj) {
    int total_length = 0;
    int alloc_size = 100;
    char *str = (char *)calloc(alloc_size, sizeof(char));
    append_str(&str, "{", &total_length, &alloc_size);

    if (json_obj->num_entries == 0) {
        goto out;
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

    free(output);

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

        free(output);
    }

out:
    append_str(&str, "}", &total_length, &alloc_size);

    return str;

fail:
    free(str);
    return NULL;
}

char *json_array_to_string(struct json_array *json_arr) {
    int total_length = 0;
    int alloc_size = 100;
    char *str = (char *)calloc(alloc_size, sizeof(char));
    append_str(&str, "[", &total_length, &alloc_size);

    if (json_arr->length == 0) {
        goto out;
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

        append_str(&str, ",", &total_length, &alloc_size);
        append_str(&str, output, &total_length, &alloc_size);

        free(output);
    }

out:
    append_str(&str, "]", &total_length, &alloc_size);

    return str;

fail:
    free(str);
    return NULL;
}

char *json_true_to_string() {
    char *str = (char *)calloc((strlen("true") + 1), sizeof(char));

    strcpy(str, "true");

    str[strlen("true")] = '\0';

    return str;
}

char *json_false_to_string() {
    char *str = (char *)calloc((strlen("false") + 1), sizeof(char));

    strcpy(str, "false");

    str[strlen("false")] = '\0';

    return str;
}

char *json_null_to_string() {
    char *str = (char *)calloc((strlen("null") + 1), sizeof(char));

    strcpy(str, "null");

    str[strlen("null")] = '\0';

    return str;
}

char *json_value_to_string(struct json_value *json_val) {
    int total_length = 0;
    int alloc_size = 100;
    char *str = (char *)calloc(alloc_size, sizeof(char *));

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
        output = json_array_to_string((struct json_array *)json_val->data);
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

int destroy_json_string(char *str) {
    int output = 0;

    // TODO: Check for errors
    free(str);

    return output;
}

int destroy_json_number(struct json_number *json_num) {
    int output = 0;

    // TODO: Check for errors
    free(json_num);

    return output;
}

int destroy_json_object(struct json_object *json_obj) {
    int output = 0;

    for (int i = 0; i < json_obj->num_entries; ++i) {
        // TODO: Uncomment me when implemented
        // struct json_value *child_value = json_object_get_value(json_obj, json_obj->keys[i]);
        // TODO: Error checking
        free(json_obj->keys[i]);

        output = destroy_json_value(json_obj->values[i]);
    }

    for (int i = json_obj->num_entries; i < json_obj->num_allocated; ++i) {
        // TODO: Error checking
        free(json_obj->keys[i]);
        // TODO: Error checking
        free(json_obj->values[i]);
    }

    // TODO: Error checking
    free(json_obj->keys);
    // TODO: Error checking
    free(json_obj->values);

    // TODO: Error checking
    free(json_obj);

    return output;
}

int destroy_json_array(struct json_array *json_arr) {
    int output = 0;

    for (int i = 0; i < json_arr->length; ++i) {
        output = destroy_json_value(json_arr->values[i]);
    }

    // TODO: Error checking
    free(json_arr->values);

    // TODO: Error checking
    free(json_arr);

    return output;
}

int destroy_json_value(struct json_value *json_val) {
    int output = 0;
    switch (json_val->type) {
    case JSON_STRING:
        output = destroy_json_string((char *)json_val->data);
        break;

    case JSON_NUMBER:
        output = destroy_json_number((struct json_number *)json_val->data);
        break;

    case JSON_OBJECT:
        output = destroy_json_object((struct json_object *)json_val->data);
        break;

    case JSON_ARRAY:
        output = destroy_json_array((struct json_array *)json_val->data);
        break;

    case JSON_TRUE:
    case JSON_FALSE:
    case JSON_NULL:
        goto out;

    case JSON_UNDEFINED:
    default:
        // TODO: Error message with reason for failure
        goto out;
    }

out:
    free(json_val);

    return output;
}
