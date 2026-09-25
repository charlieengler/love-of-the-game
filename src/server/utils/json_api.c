#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/server/utils/json_api.h"

#include "../../include/server/utils/numbers.h"
#include "../../include/server/utils/strings.h"

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

    for (int i = 0; i < json_obj->num_entries; ++i) {
        char *key = json_obj->keys[i];

        append_str(&str, "\"", &total_length, &alloc_size);
        append_str(&str, key, &total_length, &alloc_size);
        append_str(&str, "\":", &total_length, &alloc_size);

        // TODO: Error checking
        struct json_value *json_val = json_object_get_value(json_obj, key);

        char *output = json_value_to_string(json_val);

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

    for (int i = 0; i < json_arr->length; ++i) {
        struct json_value *json_val = json_arr->values[i];

        char *output = json_value_to_string(json_val);

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

    if (output) {
        // TODO: Check for errors
        append_str(&str, output, &total_length, &alloc_size);

        free(output);
    } else {
        goto fail;
    }

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
        // TODO: Error checking
        struct json_value *child_value = json_object_get_value(json_obj, json_obj->keys[i]);

        // TODO: Error checking and this is broken
        // free(json_obj->keys[i]);

        output = destroy_json_value(child_value);
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

struct json_value *create_string_json_value(char *str) {
    struct json_value *json_val = (struct json_value *)malloc(sizeof(struct json_value));

    json_val->type = JSON_STRING;
    json_val->data = str;

    return json_val;
}

struct json_value *create_number_json_value(long long integer, long long fraction, long long exponent, enum json_number_types type) {
    struct json_value *json_val = (struct json_value *)malloc(sizeof(struct json_value));

    json_val->type = JSON_NUMBER;

    struct json_number *json_num = (struct json_number *)malloc(sizeof(struct json_number));

    json_num->type = type;
    json_num->integer = integer;
    json_num->fraction = fraction;
    json_num->exponent = exponent;

    json_val->data = json_num;

    return json_val;
}

struct json_value *create_object_json_value() {
    struct json_value *json_val = (struct json_value *)malloc(sizeof(struct json_value));

    json_val->type = JSON_OBJECT;

    struct json_object *json_obj = (struct json_object *)malloc(sizeof(struct json_object));

    json_obj->keys = NULL;
    json_obj->values = NULL;

    json_obj->num_entries = 0;
    json_obj->num_allocated = 0;

    json_val->data = json_obj;

    return json_val;
}

// TODO: Hash map the object entries at some point
int json_object_add_value(struct json_object **json_obj, char *key, struct json_value *json_val) {
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

    (*json_obj)->values[(*json_obj)->num_allocated] = json_val;

    ++((*json_obj)->num_allocated);
    ++((*json_obj)->num_entries);

    return 0;
}

/*
// Credit: dbj2 by Dan Bernstein
uint64_t hash(unsigned char *str) {
    unsigned long final = 5381;
    int c;

    while ((c = *str++))
        final = ((final << 5) + final) + c; // hash * 33 + c

    return final;
}

uint64_t db_grow(struct database_mappings **mappings) {
    uint64_t new_size = (*mappings)->num_allocated * DB_GROW_MULTIPLIER;
    char **new_keys = (char **)calloc(new_size, sizeof(char *));
    struct database_entry **new_entries = (struct database_entry **)calloc(new_size, sizeof(struct database_entry *));

    struct database_mappings *new_mappings = (struct database_mappings *)malloc(sizeof(struct database_mappings));

    new_mappings->num_keys = (*mappings)->num_keys;
    new_mappings->num_entries = (*mappings)->num_entries;
    new_mappings->num_allocated = new_size;
    new_mappings->keys = new_keys;
    new_mappings->entries = new_entries;

    for (uint64_t i = 0; i < (*mappings)->num_keys; i++) {
        struct database_entry *old_entry = (*mappings)->entries[i];
        if (strcmp(old_entry->key, (*mappings)->keys[i]) != 0) {
            printf("db grow error: key and entry do not match\n");
            return 0;
        }

        db_insert(new_mappings, old_entry);
    }

    free(*mappings);
    *mappings = new_mappings;

    // TODO: Return the new number of possible mappings on success, 0 on failure
    return new_size;
}

int db_insert(struct database_mappings *mappings, struct database_entry *new_entry) {
    if (mappings->num_keys != mappings->num_entries) {
        // TODO: Error checking on the following function
        db_repair(mappings);
    }

    if (mappings->num_keys == mappings->num_allocated) {
        // TODO: The grow function causes issues
        uint64_t new_size = db_grow(&mappings);

        if (new_size == 0) {
            printf("db grow error: new_size == 0\n");
            return -1;
        } else if (new_size <= mappings->num_allocated) {
            printf("db grow error: new_size (%ld) is the same as or less than the previous size\n", new_size);
            return -1;
        }
    }

    mappings->num_keys++;
    mappings->num_entries++;

    uint64_t index = hash((unsigned char *)new_entry->key) % mappings->num_allocated;
    uint64_t num_loops = 0;
    while (mappings->keys[index]) {
        index++;
        num_loops++;

        if (num_loops > mappings->num_allocated) {
            // TODO: Maybe grow the database if this is encountered
            printf("db insert error: database is full, but didn't grow\n");
            return -1;
        }
    }

    mappings->keys[index] = new_entry->key;
    mappings->entries[index] = new_entry;

    if (mappings->num_keys == (uint64_t)(-1) || mappings->num_entries == (uint64_t)(-1)) {
        printf("db insert error: database is absolutely full somehow\n");
        return -1;
    }

    // TODO: Return 0 on success, something else otherwise
    return 0;
}

struct database_entry *db_find(struct database_mappings *mappings, char *key) {
    if (mappings->num_keys != mappings->num_entries)
        // TODO: Error checking on the following function
        db_repair(mappings);

    if (mappings->num_keys == 0) {
        printf("db find error: no keys in db\n");
        return NULL;
    }

    if (mappings->num_entries == 0) {
        printf("db find error: no entries in db\n");
        return NULL;
    }

    unsigned long index = hash((unsigned char *)key) % mappings->num_allocated;
    uint64_t num_loops = 0;
    char found_entry = 0;
    while (1) {
        ++num_loops;

        // TODO: Use a threshold value instead of the total size of mappings->num_allocated
        if (num_loops >= mappings->num_allocated) {
            // TODO: Maybe grow the database if this is encountered
            printf("db find error: could not find the given key %s\n", key);
            return NULL;
        }

        if (index >= mappings->num_allocated) {
            index = 0;
        }

        if (!mappings->keys[index]) {
            ++index;

            continue;
        }

        if (strcmp(mappings->keys[index], key) == 0) {
            found_entry = 1;

            break;
        }

        ++index;
    }

    if (!found_entry) {
        return NULL;
    }

    return mappings->entries[index];
}

int db_remove(struct database_mappings *mappings, struct database_entry *old_entry) {
    if (mappings->num_keys != mappings->num_entries) {
        // TODO: Error checking on the following function
        db_repair(mappings);
    }

    if (mappings->num_keys == 0) {
        printf("db remove error: no keys in db\n");
        return DB_REMOVE_NO_KEYS;
    }

    if (mappings->num_entries == 0) {
        printf("db remove error: no entries in db\n");
        return DB_REMOVE_NO_ENTRIES;
    }

    switch (old_entry->type) {
    case DB_STRING:
        free((old_entry->data_ptr));
        break;

    case DB_JSON:
        // TODO: Implement me
        break;

    case DB_INTEGER:
        // TODO: Implement me
        break;

    case DB_FLOAT:
        // TODO: Implement me
        break;

    case DB_UNDEFINED:
    default:
        printf("db remove error: undefined entry type\n");
        // TODO: Maybe handle this better
        free(old_entry->data_ptr);
    }

    unsigned long index = hash((unsigned char *)old_entry->key) % mappings->num_allocated;
    uint64_t num_loops = 0;
    while (1) {
        ++num_loops;

        // TODO: Use a threshold value instead of the total size of mappings->num_allocated
        if (num_loops >= mappings->num_allocated) {
            printf("db remove error: could not find the given key %s\n", old_entry->key);
            return DB_REMOVE_NOT_FOUND;
        }

        if (index >= mappings->num_allocated) {
            index = 0;
        }

        if (!mappings->keys[index]) {
            ++index;

            continue;
        }

        if (strcmp(mappings->keys[index], old_entry->key) == 0) {
            free(mappings->keys[index]);
            mappings->keys[index] = NULL;
            free(mappings->entries[index]);
            mappings->entries[index] = NULL;
            mappings->num_keys--;
            mappings->num_entries--;

            break;
        }

        ++index;
    }

    return DB_REMOVE_SUCCESS;
}
*/

struct json_value *json_object_get_value(struct json_object *json_obj, char *key) {
    // TODO: Update me when a hash map is used instead
    for (int i = 0; i < json_obj->num_entries; ++i) {
        if (!strcmp(json_obj->keys[i], key)) {
            return json_obj->values[i];
        }
    }

    // TODO: Print an error
    return NULL;
}

int json_object_remove_value(struct json_object **json_obj, char *key) {
    // TODO: Error codes for issues when removing the value

    struct json_value *json_val = json_object_get_value(*json_obj, key);
    if (!json_val) {
        // TODO: Print an error
        return -1;
    }

    --((*json_obj)->num_allocated);

    int removal_index = -1;

    char **new_keys = (char **)malloc((*json_obj)->num_allocated * sizeof(char *));
    int index = 0;
    for (int i = 0; i < (*json_obj)->num_entries; ++i) {
        if (!strcmp(key, (*json_obj)->keys[i])) {
            removal_index = i;
            continue;
        }

        new_keys[index] = (*json_obj)->keys[i];

        ++index;
    }

    free((*json_obj)->keys);

    (*json_obj)->keys = new_keys;

    struct json_value **new_values = (struct json_value **)malloc((*json_obj)->num_allocated * sizeof(struct json_value *));
    index = 0;
    for (int i = 0; i < (*json_obj)->num_entries; ++i) {
        if (i == removal_index) {
            continue;
        }

        new_values[index] = (*json_obj)->values[i];

        ++index;
    }

    free((*json_obj)->values);

    (*json_obj)->values = new_values;

    --((*json_obj)->num_entries);

    return 0;
}

struct json_value *create_array_json_value() {
    struct json_value *json_val = (struct json_value *)malloc(sizeof(struct json_value));

    json_val->type = JSON_ARRAY;

    struct json_array *json_arr = (struct json_array *)malloc(sizeof(struct json_array));

    json_arr->values = NULL;

    json_arr->length = 0;

    json_val->data = json_arr;

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
