#include <stdlib.h>
#include <string.h>

#include "../../include/server/utils/new_json_handler.h"

// TODO: This should be a hash map at some point, abstract and reuse the logic from the database
int json_object_add_value(struct json_object **json_obj, char *key, struct json_value *value) {
    // TODO: Error codes for issues when adding the value

    char **new_keys = (char **)malloc(((*json_obj)->num_allocated + 1) * sizeof(char *));
    memcpy(new_keys, (*json_obj)->keys, (*json_obj)->num_allocated * sizeof(char *));

    free((*json_obj)->keys);

    (*json_obj)->keys = new_keys;

    (*json_obj)->keys[(*json_obj)->num_allocated] = key;

    struct json_value **new_values = (struct json_value **)malloc(((*json_obj)->num_allocated + 1) * sizeof(struct json_value *));
    memcpy(new_values, (*json_obj)->values, (*json_obj)->num_allocated * sizeof(char *));

    free((*json_obj)->values);

    (*json_obj)->values = new_values;

    (*json_obj)->values[(*json_obj)->num_allocated] = value;

    ++((*json_obj)->num_allocated);
    ++((*json_obj)->num_entries);

    return 0;
}

int json_array_add_value(struct json_array **json_arr, struct json_value *value) {
    // TODO: Error codes for issues when adding the value

    struct json_value **new_values = (struct json_value **)malloc(((*json_arr)->length + 1) * sizeof(struct json_value *));
    memcpy(new_values, (*json_arr)->values, (*json_arr)->length * sizeof(char *));

    free((*json_arr)->values);

    (*json_arr)->values = new_values;

    (*json_arr)->values[(*json_arr)->length] = value;

    ++((*json_arr)->length);

    return 0;
}

int string_to_json_true(char **input) {
    if (!strncmp(*input, "true", 4)) {
        *input += 4;

        return 1;
    }

    return 0;
}

int string_to_json_false(char **input) {
    if (!strncmp(*input, "false", 5)) {
        *input += 5;

        return 0;
    }

    return -1;
}

int string_to_json_null(char **input) {
    if (!strncmp(*input, "null", 4)) {
        *input += 4;

        return 0;
    }

    return -1;
}

struct json_number *string_to_json_number(char **input) {
    struct json_number *json_num = (struct json_number *)malloc(sizeof(struct json_number));
    json_num->integer = 0;
    json_num->fraction = 0;
    json_num->exponent = 0;
    json_num->type = JSON_UNDEFINED_NUMBER;

    char sign = 1;
    char is_integer = 1;
    char is_fraction = 0;
    char exponent_sign = 1;
    char is_exponent = 0;

    if (**input == '-') {
        sign = -1;
        ++(*input);
    }

    // TODO: These allocations could get huge fast depending on the input size of the JSON string
    char *integer_str = (char *)malloc((strlen(*input) + 1) * sizeof(char));
    int int_i = 0;
    char *fraction_str = (char *)malloc((strlen(*input) + 1) * sizeof(char));
    int frac_i = 0;
    char *exponent_str = (char *)malloc((strlen(*input) + 1) * sizeof(char));
    int exp_i = 0;
    while (**input) {
        switch (**input) {
        case '.':
            if (is_fraction) {
                // TODO: Print an error for reason of failure
                goto fail;
            }

            if (is_exponent) {
                // TODO: Print an error for reason of failure
                goto fail;
            }

            is_integer = 0;
            is_fraction = 1;

            ++(*input);

            break;

        case 'E':
        case 'e':
            if (is_exponent) {
                // TODO: Print an error for reason of failure
                goto fail;
            }

            is_exponent = 1;
            is_integer = 0;
            is_fraction = 0;

            ++(*input);

            if (**input) {
                if (**input == '-') {
                    exponent_sign = -1;

                    ++(*input);
                } else if (**input == '+') {
                    ++(*input);
                }
            }

            break;

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            if (is_fraction && is_exponent) {
                // TODO: Print an error for reason of failure
                goto fail;
            }

            if (is_fraction && is_integer) {
                // TODO: Print an error for reason of failure
                goto fail;
            }

            if (is_integer && is_exponent) {
                // TODO: Print an error for reason of failure
                goto fail;
            }

            if (!is_integer && !is_fraction && !is_exponent) {
                // TODO: Print an error for reason of failure
                goto fail;
            }

            if (is_integer) {
                integer_str[int_i] = **input;

                ++int_i;
                ++(*input);
                break;
            }

            if (is_fraction) {
                fraction_str[frac_i] = **input;

                ++frac_i;
                ++(*input);
                break;
            }

            if (is_exponent) {
                exponent_str[exp_i] = **input;

                ++exp_i;
                ++(*input);
                break;
            }

            // TODO: Print an error for reason of failure
            goto fail;

        default:
            goto out;
        }
    }

out:
    integer_str[int_i] = '\0';
    fraction_str[frac_i] = '\0';
    exponent_str[exp_i] = '\0';

    // TODO: Error checking
    json_num->integer = atoi(integer_str) * sign;
    // TODO: Error checking
    json_num->fraction = atoi(fraction_str) * sign;
    // TODO: Error checking
    json_num->exponent = atoi(exponent_str) * exponent_sign;

    free(integer_str);
    free(fraction_str);
    free(exponent_str);

    if (is_integer) {
        json_num->type = JSON_INTEGER;
    }

    if (is_fraction) {
        json_num->type = JSON_FRACTION;
    }

    if (is_exponent) {
        json_num->type = JSON_EXPONENTIAL;
    }

    return json_num;

fail:
    free(integer_str);
    free(fraction_str);
    free(exponent_str);

    json_num->integer = 0;
    json_num->fraction = 0;
    json_num->exponent = 0;
    json_num->type = JSON_UNDEFINED_NUMBER;

    return json_num;
}

char *string_to_json_string(char **input) {
    // TODO: This allocation could get huge depending on the size of the input JSON string
    char *json_str = (char *)malloc((strlen(*input) + 1) * sizeof(char));

    if (**input != '"') {
        // TODO: Print an error for reason of failure
        goto fail;
    }

    ++(*input);

    int i = 0;
    int properly_terminated = 0;
    while (**input) {
        switch (**input) {
        case '\\':
            // TODO: Handle terminated characters
            break;

        case '"':
            properly_terminated = 1;
            goto out;

        default:
            json_str[i] = **input;
            ++(*input);
            break;
        }

        ++i;
    }

out:
    if (!properly_terminated) {
        // TODO: Print an error for reason of failure
        goto fail;
    }

    json_str[i] = '\0';

    char *new_json_str = (char *)malloc((strlen(json_str) + 1) * sizeof(char));
    strcpy(new_json_str, json_str);
    new_json_str[strlen(json_str)] = '\0';

    free(json_str);

    ++(*input);

    return new_json_str;

fail:
    free(json_str);
    return NULL;
}

struct json_array *string_to_json_array(char **input) {
    struct json_array *json_arr = (struct json_array *)malloc(sizeof(struct json_array));
    json_arr->values = NULL;
    json_arr->length = 0;

    if (**input != '[') {
        // TODO: Print an error for reason of failure
        goto fail;
    }

    ++(*input);

    char properly_terminated = 0;
    while (**input) {
        switch (**input) {
            JSON_WHITESPACE(input);

        case ',':
            ++(*input);
            break;

        case ']':
            ++(*input);
            properly_terminated = 1;
            goto out;

        default:
            // TODO: Maybe error check string_to_json_value, maybe handle it in json_object_add_value
            struct json_value *json_val = string_to_json_value(&(*input));

            if (json_array_add_value(&json_arr, json_val)) {
                // TODO: Print error explaining why string value could not be added
                goto fail;
            }

            break;
        }
    }

out:
    if (!properly_terminated) {
        // TODO: Print error explaining failure
        goto fail;
    }

    return json_arr;

fail:
    json_arr->values = NULL;
    json_arr->length = 0;

    return json_arr;
}

struct json_object *string_to_json_object(char **input) {
    struct json_object *json_obj = (struct json_object *)malloc(sizeof(struct json_object));
    json_obj->keys = NULL;
    json_obj->values = NULL;
    json_obj->num_allocated = 0;
    json_obj->num_entries = 0;

    if (**input != '{') {
        // TODO: Print an error for reason of failure
        goto fail;
    }

    ++(*input);

    char *current_key = NULL;
    int setting_key = 1;

    char properly_terminated = 0;
    while (**input) {
        switch (**input) {
            JSON_WHITESPACE(input);

        case ':':
            if (!setting_key) {
                // TODO: Print error on why object creation failed
                goto fail;
            }

            setting_key = 0;
            ++(*input);
            break;

        case ',':
            if (setting_key) {
                // TODO: Print error on why object creation failed
                goto fail;
            }

            setting_key = 1;
            ++(*input);
            break;

        case '}':
            properly_terminated = 1;
            ++(*input);
            goto out;

        default:
            if (setting_key) {
                char *str = string_to_json_string(&(*input));

                if (!str) {
                    // TODO: Print error stating invalid key
                    goto fail;
                } else {
                    current_key = str;
                }
            } else {
                // TODO: Maybe error check string_to_json_value, maybe handle it in json_object_add_value
                struct json_value *json_val = string_to_json_value(&(*input));

                if (json_object_add_value(&json_obj, current_key, json_val)) {
                    // TODO: Print error explaining why string value could not be added
                    goto fail;
                }
            }

            break;
        }
    }

out:
    if (!properly_terminated) {
        // TODO: Print error explaining failure
        goto fail;
    }

    return json_obj;

fail:
    json_obj->keys = NULL;
    json_obj->values = NULL;
    json_obj->num_allocated = 0;
    json_obj->num_entries = 0;

    return json_obj;
}

struct json_value *string_to_json_value(char **input) {
    struct json_value *json = (struct json_value *)malloc(sizeof(struct json_value));
    json->data = NULL;
    json->type = JSON_UNDEFINED;

    while (**input) {
        switch (**input) {
            JSON_WHITESPACE(input)

        case '"':
            char *str = string_to_json_string(input);

            json->data = str;

            if (!str) {
                json->type = JSON_NULL;
            } else {
                json->type = JSON_STRING;
            }
            break;

        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            struct json_number *json_num = string_to_json_number(input);

            if (json_num->type == JSON_UNDEFINED_NUMBER) {
                json->data = NULL;
                json->type = JSON_NULL;
            } else {
                json->data = json_num;
                json->type = JSON_NUMBER;
            }

            break;

        case '{':
            struct json_object *json_obj = string_to_json_object(input);

            if (json_obj->num_entries == 0) {
                json->data = NULL;
                json->type = JSON_NULL;
            } else {
                json->data = json_obj;
                json->type = JSON_OBJECT;
            }

            break;

        case '[':
            struct json_array *json_arr = string_to_json_array(input);

            if (json_arr->length == 0) {
                json->data = NULL;
                json->type = JSON_NULL;
            } else {
                json->data = json_arr;
                json->type = JSON_ARRAY;
            }

            break;

        case 't':
            char truthy = string_to_json_true(input);

            json->data = NULL;

            if (!truthy) {
                json->type = JSON_UNDEFINED;
            } else {
                json->type = JSON_TRUE;
            }

            break;

        case 'f':
            char falsy = string_to_json_false(input);

            json->data = NULL;

            if (falsy) {
                json->type = JSON_UNDEFINED;
            } else {
                json->type = JSON_FALSE;
            }

            break;

        case 'n':
            int null = string_to_json_null(input);

            json->data = NULL;

            if (null) {
                json->type = JSON_UNDEFINED;
            } else {
                json->type = JSON_NULL;
            }

            break;

        case '}':
        case ']':
        case ',':
            goto out;

        default:
            json->data = NULL;
            json->type = JSON_UNDEFINED;
            break;
        }
    }

out:
    return json;
}
