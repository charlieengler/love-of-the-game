#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../include/server/utils/json_api.h"

#include "./internal.h"

// Credit: dbj2 by Dan Bernstein
static uint64_t hash(unsigned char *str) {
    unsigned long final = 5381;
    int c;

    while ((c = *str++))
        final = ((final << 5) + final) + c; // hash * 33 + c

    return final;
}

char *json_value_to_string(struct json_value *json_val) { return json_value_to_string_helper(json_val, 0); }

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

    for (int i = 0; i < json_obj->num_allocated; ++i) {
        if (!json_obj->keys[i]) {
            continue;
        }

        // TODO: Error checking
        struct json_value *child_value = json_object_get_value(json_obj, json_obj->keys[i]);

        // TODO: Error checking
        free(json_obj->keys[i]);

        // TODO: Error checking
        output = destroy_json_value(child_value);
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

struct json_value *create_string_json_value(char *str) {
    struct json_value *json_val = (struct json_value *)malloc(sizeof(struct json_value));

    json_val->type = JSON_STRING;
    json_val->data = str;

    return json_val;
}

struct json_number *create_json_number() {
    struct json_number *json_num = (struct json_number *)malloc(sizeof(struct json_number));

    json_num->integer = 0;
    json_num->fraction = 0;
    json_num->exponent = 0;
    json_num->type = JSON_UNDEFINED_NUMBER;

    return json_num;
}

struct json_value *create_number_json_value(long long integer, long long fraction, long long exponent, enum json_number_types type) {
    struct json_value *json_val = (struct json_value *)malloc(sizeof(struct json_value));

    json_val->type = JSON_NUMBER;

    // TODO: Error checking
    struct json_number *json_num = create_json_number();

    json_num->type = type;
    json_num->integer = integer;
    json_num->fraction = fraction;
    json_num->exponent = exponent;

    json_val->data = json_num;

    return json_val;
}

struct json_object *create_json_object() {
    struct json_object *json_obj = (struct json_object *)malloc(sizeof(struct json_object));

    json_obj->keys = (char **)calloc(JSON_OBJECT_DEFAULT_ALLOC, sizeof(char *));
    json_obj->values = (struct json_value **)malloc(JSON_OBJECT_DEFAULT_ALLOC * sizeof(struct json_value *));

    json_obj->num_entries = 0;
    json_obj->num_allocated = JSON_OBJECT_DEFAULT_ALLOC;

    return json_obj;
}

struct json_value *create_object_json_value() {
    struct json_value *json_val = (struct json_value *)malloc(sizeof(struct json_value));

    json_val->type = JSON_OBJECT;

    // TODO: Error checking
    json_val->data = create_json_object();

    return json_val;
}

static int json_object_grow(struct json_object **json_obj) {
    int new_size = ((*json_obj)->num_allocated + 1) * JSON_OBJECT_GROW_MULTIPLIER;
    char **new_keys = (char **)calloc(new_size, sizeof(char *));

    memcpy(new_keys, (*json_obj)->keys, (*json_obj)->num_entries * sizeof(char *));

    free((*json_obj)->keys);

    struct json_value **new_values = (struct json_value **)malloc(new_size * sizeof(struct json_value *));

    memcpy(new_values, (*json_obj)->values, (*json_obj)->num_entries * sizeof(struct json_value *));

    free((*json_obj)->values);

    (*json_obj)->keys = new_keys;
    (*json_obj)->values = new_values;
    (*json_obj)->num_allocated = new_size;

    // TODO: Return the new number of possible mappings on success, 0 on failure
    return new_size;
}

// TODO: Hash map the object entries at some point
int json_object_add_value(struct json_object **json_obj, char *key, struct json_value *json_val) {
    // TODO: Error codes for issues when adding the value

    if ((*json_obj)->num_entries == (*json_obj)->num_allocated) {
        int new_size = json_object_grow(json_obj);

        if (new_size == 0) {
            printf("Grown JSON object had size zero\n");

            // TODO: Fail
        } else if (new_size <= (*json_obj)->num_allocated) {
            printf("Grown JSON object new_size (%d) is the same as or less than the previous size (%d)\n", new_size, (*json_obj)->num_allocated);

            // TODO: Fail
        }
    }

    int index = hash((unsigned char *)key) % (*json_obj)->num_allocated;
    int num_loops = 0;
    while ((*json_obj)->keys[index]) {
        if (index < (*json_obj)->num_allocated - 1) {
            ++index;
        } else {
            index = 0;
        }

        ++num_loops;

        if (num_loops > (*json_obj)->num_allocated) {
            // TODO: Maybe grow the database if this is encountered
            printf("Unable to add value to JSON object, the object didn't grow\n");

            // TODO: Fail
            break;
        }
    }

    (*json_obj)->keys[index] = (char *)calloc(strlen(key) + 1, sizeof(char));
    strcpy((*json_obj)->keys[index], key);
    (*json_obj)->values[index] = json_val;

    ++((*json_obj)->num_entries);

    if ((*json_obj)->num_entries == -1) {
        printf("Unable to add value to JSON object, it's completely full somehow\n");

        // TODO: Fail
    }

    return 0;
}

struct json_value *json_object_get_value(struct json_object *json_obj, char *key) {
    if (json_obj->num_entries == 0) {
        printf("Unable to find JSON value at %s in object, object was empty\n", key);

        // TODO: Fail
        return NULL;
    }

    int index = hash((unsigned char *)key) % json_obj->num_allocated;
    int num_loops = 0;
    while (1) {
        ++num_loops;

        if (num_loops >= json_obj->num_allocated) {
            printf("Unable to find JSON value at %s in object\n", key);

            // TODO: Fail
            break;
        }

        if (index > json_obj->num_allocated - 1) {
            index = 0;
        }

        if (!json_obj->keys[index]) {
            ++index;

            continue;
        }

        if (strcmp(json_obj->keys[index], key) == 0) {
            return json_obj->values[index];
        }

        ++index;
    }

    return NULL;
}

int json_object_remove_value(struct json_object **json_obj, char *key) {
    // TODO: Error codes for issues when removing the value

    int index = hash((unsigned char *)key) % (*json_obj)->num_allocated;
    int num_loops = 0;
    while (1) {
        ++num_loops;

        // TODO: Use a threshold value instead of the total size of mappings->num_allocated
        if (num_loops >= (*json_obj)->num_allocated) {
            printf("Unable to find JSON value at %s in object\n", key);

            // TODO: Fail
        }

        if (index >= (*json_obj)->num_allocated) {
            index = 0;
        }

        if (!(*json_obj)->keys[index]) {
            ++index;

            continue;
        }

        if (strcmp((*json_obj)->keys[index], key) == 0) {
            free((*json_obj)->keys[index]);
            (*json_obj)->keys[index] = NULL;

            free((*json_obj)->values[index]);
            (*json_obj)->values[index] = NULL;

            --((*json_obj)->num_entries);

            break;
        }

        ++index;
    }

    return 0;
}

struct json_array *create_json_array() {
    struct json_array *json_arr = (struct json_array *)malloc(sizeof(struct json_array));

    json_arr->values = NULL;
    json_arr->length = 0;

    return json_arr;
}

struct json_value *create_array_json_value() {
    struct json_value *json_val = (struct json_value *)malloc(sizeof(struct json_value));

    json_val->type = JSON_ARRAY;

    // TODO: Error checking
    json_val->data = create_json_array();

    return json_val;
}

int json_array_add_value(struct json_array **json_arr, struct json_value *json_val) {
    // TODO: Error codes for issues when adding the value

    struct json_value **new_values = (struct json_value **)malloc(((*json_arr)->length + 1) * sizeof(struct json_value *));
    memcpy(new_values, (*json_arr)->values, (*json_arr)->length * sizeof(struct json_value *));

    free((*json_arr)->values);

    (*json_arr)->values = new_values;

    (*json_arr)->values[(*json_arr)->length] = json_val;

    ++((*json_arr)->length);

    return 0;
}

struct json_value *json_array_pop_value(struct json_array *json_arr) {
    if (json_arr->length == 0) {
        return NULL;
    }

    --(json_arr->length);

    struct json_value *json_val = json_arr->values[json_arr->length];

    struct json_value **new_values = (struct json_value **)malloc((json_arr->length) * sizeof(struct json_value *));
    memcpy(new_values, json_arr->values, (json_arr->length) * sizeof(struct json_value *));

    free(json_arr->values);

    json_arr->values = new_values;

    return json_val;
}

struct json_value *create_true_json_value() {
    struct json_value *json_val = (struct json_value *)malloc(sizeof(struct json_value));

    json_val->type = JSON_TRUE;
    json_val->data = NULL;

    return json_val;
}

struct json_value *create_false_json_value() {
    struct json_value *json_val = (struct json_value *)malloc(sizeof(struct json_value));

    json_val->type = JSON_FALSE;
    json_val->data = NULL;

    return json_val;
}

struct json_value *create_null_json_value() {
    struct json_value *json_val = (struct json_value *)malloc(sizeof(struct json_value));

    json_val->type = JSON_NULL;
    json_val->data = NULL;

    return json_val;
}
