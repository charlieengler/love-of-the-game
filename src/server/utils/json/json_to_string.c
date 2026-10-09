#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../include/logging.h"

#include "../../../include/server/utils/json.h"

#include "../../../include/server/utils/numbers.h"
#include "../../../include/server/utils/strings.h"

#include "internal.h"

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

char *json_object_to_string(struct json_object *json_obj, int depth) {
    int total_length = 0;
    int alloc_size = 100;
    char *str = (char *)calloc(alloc_size, sizeof(char));
    int append_out = append_str(&str, "{", &total_length, &alloc_size);
    if (append_out) {
        printd("json_to_string.c->json_object_to_string(): append_str() returned %d on call %d\n", append_out, 1);

        goto fail;
    }

    if (json_obj->num_entries == 0) {
        goto out;
    }

    int total_entries = 0;
    for (int i = 0; i < json_obj->num_allocated; ++i) {
        if (!json_obj->keys[i]) {
            continue;
        }

        ++total_entries;

        append_out = append_str(&str, "\n", &total_length, &alloc_size);
        if (append_out) {
            printd("json_to_string.c->json_object_to_string(): append_str() returned %d on call %d\n", append_out, 2);

            goto fail;
        }

        for (int j = 0; j < depth + 1; ++j) {
            append_out = append_str(&str, "    ", &total_length, &alloc_size);
            if (append_out) {
                printd("json_to_string.c->json_object_to_string(): append_str() returned %d on call %d\n", append_out, 3);

                goto fail;
            }
        }

        char *key = json_obj->keys[i];

        append_out = append_str(&str, "\"", &total_length, &alloc_size);
        if (append_out) {
            printd("json_to_string.c->json_object_to_string(): append_str() returned %d on call %d\n", append_out, 4);

            goto fail;
        }
        append_out = append_str(&str, key, &total_length, &alloc_size);
        if (append_out) {
            printd("json_to_string.c->json_object_to_string(): append_str() returned %d on call %d\n", append_out, 5);

            goto fail;
        }
        append_out = append_str(&str, "\": ", &total_length, &alloc_size);
        if (append_out) {
            printd("json_to_string.c->json_object_to_string(): append_str() returned %d on call %d\n", append_out, 6);

            goto fail;
        }

        struct json_value *json_val = json_object_get_value(json_obj, key);
        if (!json_val) {
            printd("json_to_string.c->json_object_to_string(): json_val was null when getting json_obj value at %s\n", key);

            goto fail;
        }

        char *output = json_value_to_string_helper(json_val, depth + 1);

        if (!output) {
            printd("json_to_string.c->json_object_to_string(): output was null when converting json_val to string\n");

            goto fail;
        }

        append_out = append_str(&str, output, &total_length, &alloc_size);
        if (append_out) {
            printd("json_to_string.c->json_object_to_string(): append_str() returned %d on call %d\n", append_out, 7);

            goto fail;
        }

        if (total_entries < json_obj->num_entries) {
            append_out = append_str(&str, ",", &total_length, &alloc_size);
            if (append_out) {
                printd("json_to_string.c->json_object_to_string(): append_str() returned %d on call %d\n", append_out, 8);

                goto fail;
            }
        }

        free(output);
    }

    append_out = append_str(&str, "\n", &total_length, &alloc_size);
    if (append_out) {
        printd("json_to_string.c->json_object_to_string(): append_str() returned %d on call %d\n", append_out, 9);

        goto fail;
    }

    for (int j = 0; j < depth; ++j) {
        append_out = append_str(&str, "    ", &total_length, &alloc_size);
        if (append_out) {
            printd("json_to_string.c->json_object_to_string(): append_str() returned %d on call %d\n", append_out, 10);

            goto fail;
        }
    }

out:
    append_out = append_str(&str, "}", &total_length, &alloc_size);
    if (append_out) {
        printd("json_to_string.c->json_object_to_string(): append_str() returned %d on call %d\n", append_out, 11);

        goto fail;
    }

    return str;

fail:
    free(str);
    return NULL;
}

char *json_array_to_string(struct json_array *json_arr, int depth) {
    int total_length = 0;
    int alloc_size = 100;
    char *str = (char *)calloc(alloc_size, sizeof(char));
    int append_out = append_str(&str, "[", &total_length, &alloc_size);
    if (append_out) {
        printd("json_to_string.c->json_array_to_string(): append_str() returned %d on call %d\n", append_out, 1);

        goto fail;
    }

    if (json_arr->length == 0) {
        goto out;
    }

    for (int i = 0; i < json_arr->length; ++i) {
        append_out = append_str(&str, "\n", &total_length, &alloc_size);
        if (append_out) {
            printd("json_to_string.c->json_array_to_string(): append_str() returned %d on call %d\n", append_out, 2);

            goto fail;
        }

        for (int j = 0; j < depth + 1; ++j) {
            append_out = append_str(&str, "    ", &total_length, &alloc_size);
            if (append_out) {
                printd("json_to_string.c->json_array_to_string(): append_str() returned %d on call %d\n", append_out, 3);

                goto fail;
            }
        }

        struct json_value *json_val = json_arr->values[i];

        char *output = json_value_to_string_helper(json_val, depth + 1);

        if (!output) {
            printd("json_to_string.c->json_array_to_string(): json_value_to_string_helper() returned NULL on JSON array entry %d\n", i);

            goto fail;
        }

        append_out = append_str(&str, output, &total_length, &alloc_size);
        if (append_out) {
            printd("json_to_string.c->json_array_to_string(): append_str() returned %d on call %d\n", append_out, 4);

            goto fail;
        }

        if (i + 1 < json_arr->length) {
            append_out = append_str(&str, ",", &total_length, &alloc_size);
            if (append_out) {
                printd("json_to_string.c->json_array_to_string(): append_str() returned %d on call %d\n", append_out, 5);

                goto fail;
            }
        }

        free(output);
    }

    append_out = append_str(&str, "\n", &total_length, &alloc_size);
    if (append_out) {
        printd("json_to_string.c->json_array_to_string(): append_str() returned %d on call %d\n", append_out, 6);

        goto fail;
    }

    for (int j = 0; j < depth; ++j) {
        append_out = append_str(&str, "    ", &total_length, &alloc_size);
        if (append_out) {
            printd("json_to_string.c->json_array_to_string(): append_str() returned %d on call %d\n", append_out, 7);

            goto fail;
        }
    }

out:
    append_out = append_str(&str, "]", &total_length, &alloc_size);
    if (append_out) {
        printd("json_to_string.c->json_array_to_string(): append_str() returned %d on call %d\n", append_out, 8);

        goto fail;
    }

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

char *json_value_to_string_helper(struct json_value *json_val, int depth) {
    int total_length = 0;
    int alloc_size = 100;
    char *str = (char *)calloc(alloc_size, sizeof(char *));

    // TODO: Destroy the JSON related structs as they are added to the string, or destroy the whole value at the end of the function call
    char *output = NULL;
    switch (json_val->type) {
    case JSON_STRING:
        output = json_string_to_string((char *)json_val->data);

        if (!output) {
            printd("json_to_string.c->json_value_to_string_helper(): json_string_to_string output was NULL\n");

            goto fail;
        }
        break;

    case JSON_NUMBER:
        output = json_number_to_string((struct json_number *)json_val->data);

        if (!output) {
            printd("json_to_string.c->json_value_to_string_helper(): json_number_to_string output was NULL\n");

            goto fail;
        }
        break;

    case JSON_OBJECT:
        output = json_object_to_string((struct json_object *)json_val->data, depth);

        if (!output) {
            printd("json_to_string.c->json_value_to_string_helper(): json_object_to_string output was NULL\n");

            goto fail;
        }
        break;

    case JSON_ARRAY:
        output = json_array_to_string((struct json_array *)json_val->data, depth);

        if (!output) {
            printd("json_to_string.c->json_value_to_string_helper(): json_array_to_string output was NULL\n");

            goto fail;
        }
        break;

    case JSON_TRUE:
        output = json_true_to_string();

        if (!output) {
            printd("json_to_string.c->json_value_to_string_helper(): json_true_to_string output was NULL\n");

            goto fail;
        }
        break;

    case JSON_FALSE:
        output = json_false_to_string();

        if (!output) {
            printd("json_to_string.c->json_value_to_string_helper(): json_false_to_string output was NULL\n");

            goto fail;
        }
        break;

    case JSON_NULL:
        output = json_null_to_string();

        if (!output) {
            printd("json_to_string.c->json_value_to_string_helper(): json_null_to_string output was NULL\n");

            goto fail;
        }
        break;

    case JSON_UNDEFINED:
    default:
        printd("json_to_string.c->json_value_to_string_helper(): JSON value had type undefined\n");

        goto fail;
    }

    if (!output) {
        printd("json_to_string.c->json_value_to_string_helper(): NULL output detected\n");

        goto fail;
    }

    int append_out = append_str(&str, output, &total_length, &alloc_size);
    if (append_out) {
        printd("json_to_string.c->json_value_to_string_helper(): append_str() returned %d\n", append_out);

        goto fail;
    }

    free(output);

    return str;

fail:
    free(str);
    return NULL;
}
