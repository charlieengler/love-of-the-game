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
    // This includes the opening and closing parentheses on any object and the newline at the beginning of the object
    json->string_size = 3;

    return json;
}

// TODO: Throw a hash map at this in the future
int json_add_entry(struct json_object *json, char *key, char *val) {
    char **new_keys = (char**)malloc((++(json->num_keys)) * sizeof(char*));
    char **new_vals = (char**)malloc((++(json->num_vals)) * sizeof(char*));

    if(json->num_keys != json->num_vals)
        // TODO: Error handling on this function
        json_repair_object(json);

    for(int i = 0; i < json->num_keys - 1; i++) {
        new_keys[i] = json->keys[i];
        new_vals[i] = json->vals[i];
    }

    // TODO: We are assuming that the passed key and val have been preallocated
    new_keys[json->num_keys-1] = key;
    new_vals[json->num_vals-1] = val;

    // Length of the key plus opening double quotes, closing double quotes, and the colon between the key and value
    json->string_size += strlen(key) + 3;
    // Length of the value plus opening double quotes, closing double quotes, the comma between entries, and the newline character
    json->string_size += strlen(val) + 4;

    json->keys = new_keys;
    json->vals = new_vals;

    // TODO: Return 0 on success, something else on failure
    return 0;
}

char *json_find_entry(struct json_object *json, char *key) {
    if(json->num_keys != json->num_vals)
        // TODO: Error handling on this function
        json_repair_object(json);

    for(int i = 0; i < json->num_keys; i++)
        if(strcmp(json->keys[i], key) == 0)
            return json->vals[i];

    return NULL;
}

char *json_to_string(struct json_object *json) {
    if(json->num_keys != json->num_vals)
        // TODO: Error handling on this function
        json_repair_object(json);

    char *final_string = (char*)malloc((json->string_size + 1) * sizeof(char));
    char *return_string = final_string;

    strcpy(final_string, "{\n");
    final_string += strlen("{\n");

    for(int i = 0; i < json->num_keys; i++) {
        *(final_string++) = '"';

        strcpy(final_string, json->keys[i]);
        final_string += strlen(json->keys[i]);

        *(final_string++) = '"';
        *(final_string++) = ':';
        *(final_string++) = '"';

        strcpy(final_string, json->vals[i]);
        final_string += strlen(json->vals[i]);

        *(final_string++) = '"';
        *(final_string++) = ',';
        *(final_string++) = '\n';
    }

    // Reverse back up to erase the previous comma, also overwrites the newline
    final_string -= 2;

    *(final_string++) = '\n';
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