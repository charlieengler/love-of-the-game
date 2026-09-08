#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/server/utils/json_handler.h"

// TODO: This function should find keys with missing values and either restore
// the values to defaults, or just discard the keys
int json_repair_object(struct json_object *json) {
    printf("JSON Object key and value numbers do not align on object %p\n", json);

    return 0;
}

struct json_object *json_initialize_object() {
    struct json_object *json = (struct json_object *)malloc(sizeof(struct json_object));

    json->num_keys = 0;
    json->num_vals = 0;
    // This includes the opening and closing parentheses on any object (null
    // terminating character is handled in the to_string function)
    json->string_size = 2;

    return json;
}

// TODO: Throw a hash map at this in the future
int json_add_entry(struct json_object *json_obj, char *key, struct json_value *val) {
    char **new_keys = (char **)malloc((++(json_obj->num_keys)) * sizeof(char *));
    struct json_value **new_vals = (struct json_value **)malloc((++(json_obj->num_vals)) * sizeof(struct json_value *));

    if (json_obj->num_keys != json_obj->num_vals) {
        // TODO: Error handling on this function
        json_repair_object(json_obj);
    }

    for (uint64_t i = 0; i < json_obj->num_keys - 1; i++) {
        new_keys[i] = json_obj->keys[i];
        new_vals[i] = json_obj->vals[i];
    }

    // TODO: We are assuming that the passed key and val have been preallocated
    new_keys[json_obj->num_keys - 1] = key;
    new_vals[json_obj->num_vals - 1] = val;

    // Length of the key plus opening double quotes, closing double quotes, and
    // the colon between the key and value
    json_obj->string_size += strlen(key) + 3;

    switch (val->type) {
    case JSON_STRING:
        // +2 for the quotes
        json_obj->string_size += strlen(val->str_val) + 2;
        break;

    case JSON_FORMATTED_STRING:
        // This should be a string that is already formatted in the JSON format
        json_obj->string_size += strlen(val->str_val);
        break;

    case JSON_NESTED:
        json_obj->string_size += val->child_object->string_size;
        break;

    case JSON_INTEGER:
        // +1 to allow space for val
        json_obj->string_size += ceil(log10(val->int_val)) + 1;
        break;

    case JSON_FLOAT:
        // It seems like 327 should be enough to support a rounded float
        json_obj->string_size += 327;
        break;

    case JSON_UNDEFINED:
        printf("Undefined json value type when adding entry\n");
        break;
    }

    // Opening and closing double quotes (if applicable), as well as the comma
    // between entries
    json_obj->string_size += 3;

    json_obj->keys = new_keys;
    json_obj->vals = new_vals;

    // TODO: Return 0 on success, something else on failure
    return 0;
}

// If an entry already exists, just return the result of json_find_entry. Otherwise, create it
struct json_value *json_add_string_entry(struct json_object *json_obj, char *key, char *init_val) {
    struct json_value *entry = json_find_entry(json_obj, key);

    if (entry) {
        return entry;
    }

    unsigned long init_val_len = strlen(init_val);

    entry = (struct json_value *)malloc(sizeof(struct json_value));
    entry->type = JSON_STRING;
    entry->str_val = (char *)malloc(init_val_len + 1);
    strcpy(entry->str_val, init_val);
    entry->str_val[init_val_len] = '\0';

    json_add_entry(json_obj, key, entry);

    return entry;
}

struct json_value *json_find_entry(struct json_object *json_obj, char *key) {
    if (json_obj->num_keys != json_obj->num_vals)
        // TODO: Error handling on this function
        json_repair_object(json_obj);

    for (uint64_t i = 0; i < json_obj->num_keys; i++)
        if (strcmp(json_obj->keys[i], key) == 0)
            return json_obj->vals[i];

    return NULL;
}

int json_add_entry_from_string(struct json_object *json_obj, char *raw_string, uint64_t total_len) {
    char open_string = 0;

    char *key = NULL;
    char *val = NULL;
    struct json_value *json_val;
    char *string_start = raw_string;
    uint64_t current_len = 0;
    while (current_len < total_len) {
        switch (*raw_string) {
        case '"':
            if (open_string) {
                if (!key) {
                    key = malloc(sizeof(char) * (current_len + 1));
                    memcpy(key, string_start, current_len);
                    key[current_len - 1] = '\0';
                } else {
                    val = malloc(sizeof(char) * (current_len + 1));
                    memcpy(val, string_start, current_len);
                    val[current_len - 1] = '\0';
                }

                open_string = 0;
            } else {
                string_start = (raw_string + 1);
                current_len = 0;
                open_string = 1;
            }
            break;
        case ',':
            if (open_string) {
                ++current_len;
                ++raw_string;
                continue;
            }

            json_val = (struct json_value *)malloc(sizeof(struct json_value));
            json_val->str_val = val;
            json_val->type = JSON_STRING;

            json_add_entry(json_obj, key, json_val);

            key = NULL;
            val = NULL;
            json_val = NULL;
            current_len = 0;
            open_string = 0;
            break;
        case '}':
            if (open_string) {
                ++current_len;
                ++raw_string;
                continue;
            }

            json_val = (struct json_value *)malloc(sizeof(struct json_value));
            json_val->str_val = val;
            json_val->type = JSON_STRING;

            json_add_entry(json_obj, key, json_val);

            return 0;
            break;
        default:
            break;
        }

        ++current_len;
        ++raw_string;
    }

    return 0;
}

struct json_object *json_parse_string(char *input) {
    char *cleaned_input = (char *)calloc(strlen(input), sizeof(char));
    char *cleaned_input_start = cleaned_input;
    uint64_t cleaned_input_len = 0;
    while (*input) {
        if (*input != '\n') {
            *cleaned_input = *input;
            ++cleaned_input_len;
        }

        ++cleaned_input;
        ++input;
    }

    cleaned_input = cleaned_input_start;

    struct json_object *return_object = json_initialize_object();

    return_object->string_size = cleaned_input_len;

    uint64_t current_len = 0;
    while (*cleaned_input) {
        ++current_len;

        switch (*cleaned_input) {
        case '{':
            current_len = 0;
            break;

        case '}':
            json_add_entry_from_string(return_object, cleaned_input - current_len, current_len);
            current_len = 0;
            break;
        }

        ++cleaned_input;
    }

    // TODO: Special syntax for a newly parsed json object that needs to be
    // repaired
    if (return_object->num_keys != return_object->num_vals)
        // TODO: Error handling on this function
        json_repair_object(return_object);

    return return_object;
}

char *json_encode_val(struct json_value *json_val) {
    char *return_string = NULL;
    uint64_t val_len = 0;

    switch (json_val->type) {
    case JSON_STRING:
        // +3 for the quotes and the null terminating character
        val_len = (uint64_t)strlen(json_val->str_val);
        return_string = (char *)calloc(val_len + 3, sizeof(char));

        strcpy(return_string + 1, json_val->str_val);
        return_string[0] = '"';
        return_string[val_len + 1] = '"';

        break;

    case JSON_FORMATTED_STRING:
        return_string = (char *)calloc(strlen(json_val->str_val) + 1, sizeof(char));

        strcpy(return_string, json_val->str_val);
        break;

    case JSON_NESTED:
        return json_to_string(json_val->child_object);

    case JSON_INTEGER:
        // +2 to allow space for val and null terminating character
        return_string = (char *)calloc(ceil(log10(json_val->int_val)) + 2, sizeof(char));

        sprintf(return_string, "%ld", json_val->int_val);

        break;

    case JSON_FLOAT:
        // It seems like 327 should be enough to support a rounded float
        return_string = (char *)calloc(327, sizeof(char));

        sprintf(return_string, "%.16f", json_val->float_val);

        break;

    case JSON_UNDEFINED:
        printf("Attempted to encode undefined JSON value\n");
        break;
    }

    return return_string;
}

char *json_to_string(struct json_object *json_obj) {
    if (json_obj->num_keys != json_obj->num_vals)
        // TODO: Error handling on this function
        json_repair_object(json_obj);

    char *final_string = (char *)malloc((json_obj->string_size + 1) * sizeof(char));
    char *return_string = final_string;

    *(final_string++) = '{';

    for (uint64_t i = 0; i < json_obj->num_keys; i++) {
        *(final_string++) = '"';

        strcpy(final_string, json_obj->keys[i]);
        final_string += strlen(json_obj->keys[i]);

        *(final_string++) = '"';
        *(final_string++) = ':';

        char *encoded_val = json_encode_val(json_obj->vals[i]);
        strcpy(final_string, encoded_val);
        final_string += strlen(encoded_val);

        *(final_string++) = ',';
    }

    // Reverse back up to erase the previous comma
    final_string -= 1;

    *(final_string++) = '}';
    *(final_string++) = 0;

    return return_string;
}

int json_destroy_object(struct json_object *json) {
    if (json->num_keys != json->num_vals)
        // TODO: Error handling on this function
        json_repair_object(json);

    // TODO: Free the keys and vals
    // TODO: Free the pointers to all of the keys and vals
    // TODO: Free the final json object pointer as this should be allocated

    // TODO: Return 0 on success, something else on failure
    return 0;
}
