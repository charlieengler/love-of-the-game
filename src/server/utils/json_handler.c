#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/server/utils/json_handler.h"

// TODO: This function should find keys with missing values and either restore the values to defaults, or just discard the keys
int json_repair_object(struct json_object *json) {
    printf("JSON Object key and value numbers do not align\n");

    return 0;
}

struct json_object *json_initialize_object() {
    struct json_object *json = (struct json_object*)malloc(sizeof(struct json_object));

    json->num_keys = 0;
    json->num_vals = 0;
    // This includes the opening and closing parentheses on any object (null terminating character is handled in the to_string function)
    json->string_size = 2;

    return json;
}

// TODO: Throw a hash map at this in the future
int json_add_entry(struct json_object *json_obj, char *key, struct json_value *val) {
    char **new_keys = (char**)malloc((++(json_obj->num_keys)) * sizeof(char*));
    struct json_value **new_vals = (struct json_value**)malloc((++(json_obj->num_vals)) * sizeof(struct json_value*));

    if(json_obj->num_keys != json_obj->num_vals)
        // TODO: Error handling on this function
        json_repair_object(json_obj);

    for(int i = 0; i < json_obj->num_keys - 1; i++) {
        new_keys[i] = json_obj->keys[i];
        new_vals[i] = json_obj->vals[i];
    }

    // TODO: We are assuming that the passed key and val have been preallocated
    new_keys[json_obj->num_keys-1] = key;
    new_vals[json_obj->num_vals-1] = val;

    // Length of the key plus opening double quotes, closing double quotes, and the colon between the key and value
    json_obj->string_size += strlen(key) + 3;
    
    switch(val->type) {
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
    }

    // Opening and closing double quotes (if applicable), as well as the comma between entries
    json_obj->string_size += 3;

    json_obj->keys = new_keys;
    json_obj->vals = new_vals;

    // TODO: Return 0 on success, something else on failure
    return 0;
}

struct json_value *json_find_entry(struct json_object *json_obj, char *key) {
    if(json_obj->num_keys != json_obj->num_vals)
        // TODO: Error handling on this function
        json_repair_object(json_obj);

    for(int i = 0; i < json_obj->num_keys; i++)
        if(strcmp(json_obj->keys[i], key) == 0)
            return json_obj->vals[i];

    return NULL;
}

struct json_object *json_parse_string(char *input) {
    struct json_object *return_object = json_initialize_object();

    // TODO: Implement me

    // TODO: Special syntax for a newly parsed json object that needs to be repaired
    if(return_object->num_keys != return_object->num_vals)
        // TODO: Error handling on this function
        json_repair_object(return_object);

    return return_object;
}

char *json_encode_val(struct json_value *json_val) {
    char *return_string = NULL;

    switch(json_val->type) {
        case JSON_STRING:
            // +3 for the quotes and the null terminating character
            uint64_t val_len = strlen(json_val->str_val);
            return_string = (char*)calloc(val_len + 3, sizeof(char));

            strcpy(return_string+1, json_val->str_val);
            return_string[0] = '"';
            return_string[val_len+1] = '"';

            break;

        case JSON_FORMATTED_STRING:
            return_string = (char*)calloc(strlen(json_val->str_val) + 1, sizeof(char));

            strcpy(return_string, json_val->str_val);
            break;

        case JSON_NESTED:
            return json_to_string(json_val->child_object);

        case JSON_INTEGER:
            // +2 to allow space for val and null terminating character
            return_string = (char*)calloc(ceil(log10(json_val->int_val)) + 2, sizeof(char));

            sprintf(return_string, "%ld", json_val->int_val);

            break;

        case JSON_FLOAT:
            // It seems like 327 should be enough to support a rounded float
            return_string = (char*)calloc(327, sizeof(char));

            sprintf(return_string, "%.16f", json_val->float_val);

            break;
    }

    return return_string;
}

char *json_to_string(struct json_object *json_obj) {
    if(json_obj->num_keys != json_obj->num_vals)
        // TODO: Error handling on this function
        json_repair_object(json_obj);

    char *final_string = (char*)malloc((json_obj->string_size + 1) * sizeof(char));
    char *return_string = final_string;

    *(final_string++) = '{';

    for(int i = 0; i < json_obj->num_keys; i++) {
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
    if(json->num_keys != json->num_vals)
        // TODO: Error handling on this function
        json_repair_object(json);

    // TODO: Free the keys and vals
    // TODO: Free the pointers to all of the keys and vals
    // TODO: Free the final json object pointer as this should be allocated

    // TODO: Return 0 on success, something else on failure
    return 0;
}