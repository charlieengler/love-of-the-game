#include <stdlib.h>
#include <string.h>

#include "./internal.h"

#include "../../../include/logging.h"

#include "../../../include/server/utils/json.h"

int string_to_json_true(char **input) {
    char *start = *input;

    if (!strncmp(*input, "true", 4)) {
        *input += 4;

        return 1;
    }

    printd("json_parser.c->string_to_json_true(): expected \"true\", string %s didn't match\n", start);

    return 0;
}

int string_to_json_false(char **input) {
    char *start = *input;

    if (!strncmp(*input, "false", 5)) {
        *input += 5;

        return 0;
    }

    printd("json_parser.c->string_to_json_false(): expected \"false\", string %s didn't match\n", start);

    return -1;
}

int string_to_json_null(char **input) {
    char *start = *input;

    if (!strncmp(*input, "null", 4)) {
        *input += 4;

        return 0;
    }

    printd("json_parser.c->string_to_json_null(): expected \"null\", string %s didn't match\n", start);

    return -1;
}

struct json_number *string_to_json_number(char **input) {
    struct json_number *json_num = create_json_number();
    if (!json_num) {
        printd("json_parser.c->string_to_json_number(): create_json_number() returned NULL\n");

        return NULL;
    }

    char *start = *input;

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
                printd("json_parser.c->string_to_json_number(): is_fraction already set on input %s\n", start);

                goto fail;
            }

            if (is_exponent) {
                printd("json_parser.c->string_to_json_number(): is_exponent already set on input %s\n", start);

                goto fail;
            }

            is_integer = 0;
            is_fraction = 1;

            ++(*input);

            break;

        case 'E':
        case 'e':
            if (is_exponent) {
                printd("json_parser.c->string_to_json_number(): is_exponent already set on input %s\n", start);

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
                printd("json_parser.c->string_to_json_number(): is_fraction and is_exponent both set on input %s\n", start);

                goto fail;
            }

            if (is_fraction && is_integer) {
                printd("json_parser.c->string_to_json_number(): is_fraction and is_integer both set on input %s\n", start);

                goto fail;
            }

            if (is_integer && is_exponent) {
                printd("json_parser.c->string_to_json_number(): is_integer and is_exponent both set on input %s\n", start);

                goto fail;
            }

            if (!is_integer && !is_fraction && !is_exponent) {
                printd("json_parser.c->string_to_json_number(): is_integer, is_fraction, and is_exponent all unset on input %s\n", start);

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

            printd("json_parser.c->string_to_json_number(): failed to add digit on input %s\n", start);
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
    free(json_num);

    return NULL;
}

char *string_to_json_string(char **input) {
    char *start = *input;

    // TODO: This allocation could get huge depending on the size of the input JSON string
    char *json_str = (char *)malloc((strlen(*input) + 1) * sizeof(char));

    if (**input != '"') {
        printd("json_parser.c->string_to_json_string(): JSON string started without quotes on input %s\n", start);
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
        printd("json_parser.c->string_to_json_string(): JSON string improperly terminated on input %s\n", start);

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
    char *start = *input;

    struct json_array *json_arr = create_json_array();
    if (!json_arr) {
        printd("json_parser.c->string_to_json_array(): create_json_array() returned NULL\n");

        return NULL;
    }

    if (**input != '[') {
        printd("json_parser.c->string_to_json_array(): JSON array started without a square bracket on input %s\n", start);

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
            struct json_value *json_val = string_to_json_value(&(*input));
            if (!json_val) {
                printd("json_parser.c->string_to_json_array(): string_to_json_value() was NULL on input %s\n", start);

                goto fail;
            }

            int add_res = json_array_add_value(&json_arr, json_val);

            if (add_res) {
                printd("json_parser.c->string_to_json_array(): json_array_add_value() returned %d on input %s\n", add_res, start);

                goto fail;
            }

            break;
        }
    }

out:
    if (!properly_terminated) {
        printd("json_parser.c->string_to_json_array(): JSON array string improperly terminated on input %s\n", start);

        goto fail;
    }

    return json_arr;

fail:
    json_arr->values = NULL;
    json_arr->length = 0;

    return json_arr;
}

struct json_object *string_to_json_object(char **input) {
    char *start = *input;

    // TODO: Error checking
    struct json_object *json_obj = create_json_object();
    if (!json_obj) {
        printd("json_parser.c->string_to_json_object(): create_json_object() returned NULL\n");

        return NULL;
    }

    if (**input != '{') {
        printd("json_parser.c->string_to_json_object(): JSON array started without a curly brace on input %s\n", start);

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
                printd("json_parser.c->string_to_json_object(): improper key format on input %s\n", start);

                goto fail;
            }

            setting_key = 0;
            ++(*input);
            break;

        case ',':
            if (setting_key) {
                printd("json_parser.c->string_to_json_object(): improper value format on input %s\n", start);

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
                    printd("json_parser.c->string_to_json_object(): unable to parse key string on input %s\n", start);

                    goto fail;
                } else {
                    current_key = str;
                }
            } else {
                // TODO: Maybe error check string_to_json_value, maybe handle it in json_object_add_value
                struct json_value *json_val = string_to_json_value(&(*input));
                if (!json_val) {
                    printd("json_parser.c->string_to_json_object(): string_to_json_value() returned NULL on input %s\n", start);

                    goto fail;
                }

                int add_res = json_object_add_value(&json_obj, current_key, json_val);

                if (add_res) {
                    printd("json_parser.c->string_to_json_object(): json_object_add_value() returned %d on input %s\n", add_res, start);

                    goto fail;
                }
            }

            break;
        }
    }

out:
    if (!properly_terminated) {
        printd("json_parser.c->string_to_json_object(): JSON object string improperly terminated on input %s\n", start);

        goto fail;
    }

    return json_obj;

fail:
    int destroy_res = destroy_json_object(json_obj);
    if (destroy_res) {
        printd("json_parser.c->string_to_json_object(): destroy_json_object() returned %d on input %s\n", destroy_res, start);
    }

    return NULL;
}

struct json_value *string_to_json_value(char **input) {
    char *start = *input;

    struct json_value *json = (struct json_value *)malloc(sizeof(struct json_value));
    json->data = NULL;
    json->type = JSON_UNDEFINED;

    while (**input) {
        switch (**input) {
            JSON_WHITESPACE(input)

        case '"':
            char *str = string_to_json_string(input);
            if (!str) {
                printd("json_parser.c->string_to_json_value(): string_to_json_string() returned NULL on input %s\n", start);

                goto fail;
            }

            json->type = JSON_STRING;
            json->data = str;

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
            if (!json_num) {
                printd("json_parser.c->string_to_json_value(): string_to_json_number() returned NULL on input %s\n", start);

                goto fail;
            }

            if (json_num->type == JSON_UNDEFINED_NUMBER) {
                printd("json_parser.c->string_to_json_value(): string_to_json_number() was an undefined number type on input %s\n", start);

                goto fail;
            }

            json->type = JSON_NUMBER;
            json->data = json_num;

            break;

        case '{':
            struct json_object *json_obj = string_to_json_object(input);
            if (!json_obj) {
                printd("json_parser.c->string_to_json_value(): string_to_json_object() returned NULL on input %s\n", start);

                goto fail;
            }

            json->type = JSON_OBJECT;
            json->data = json_obj;

            break;

        case '[':
            struct json_array *json_arr = string_to_json_array(input);
            if (!json_arr) {
                printd("json_parser.c->string_to_json_value(): string_to_json_array() returned NULL on input %s\n", start);

                goto fail;
            }

            json->type = JSON_ARRAY;
            json->data = json_arr;

            break;

        case 't':
            char truthy = string_to_json_true(input);
            if (truthy != 1) {
                printd("json_parser.c->string_to_json_value(): string_to_json_true() returned %d on input %s\n", truthy, start);

                goto fail;
            }

            json->type = JSON_TRUE;
            json->data = NULL;

            break;

        case 'f':
            char falsy = string_to_json_false(input);
            if (falsy) {
                printd("json_parser.c->string_to_json_value(): string_to_json_false() returned %d on input %s\n", falsy, start);

                goto fail;
            }

            json->type = JSON_FALSE;
            json->data = NULL;

            break;

        case 'n':
            int null = string_to_json_null(input);
            if (null) {
                printd("json_parser.c->string_to_json_value(): string_to_json_null() returned %d on input %s\n", null, start);

                goto fail;
            }

            json->type = JSON_NULL;
            json->data = NULL;

            break;

        case '}':
        case ']':
        case ',':
            goto out;

        default:
            printd("json_parser.c->string_to_json_value(): undefined JSON type on input %s\n", start);

            json->data = NULL;
            json->type = JSON_UNDEFINED;

            break;
        }
    }

out:
    return json;

fail:
    free(json);

    return NULL;
}
