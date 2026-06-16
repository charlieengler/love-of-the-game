#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/server/utils/json_handler.h"
#include "tests.h"

static int test_json_initialize_object() {
    struct json_object *test_object;

    test_object = json_initialize_object();

    if (!test_object) {
        printf("[TEST FAIL] test_json_initialize_object(): new_object is null\n");

        return 1;
    }

    if (test_object->num_keys != 0) {
        printf("[TEST FAIL] test_json_initialize_object(): new_object->num_keys %ld does not equal expected value (0)\n", test_object->num_keys);

        return 1;
    }

    if (test_object->num_vals != 0) {
        printf("[TEST FAIL] test_json_initialize_object(): new_object->num_vals %ld does not equal expected value (0)\n", test_object->num_vals);

        return 1;
    }

    if (test_object->string_size != 2) {
        printf("[TEST FAIL] test_json_initialize_object(): new_object->string_size %ld does not equal expected value (2)\n", test_object->string_size);

        return 1;
    }

    return 0;
}

static int test_json_add_entry() {
    struct json_object *test_object = json_initialize_object();

    uint64_t expected_str_size = 2;

    // TODO: Only testing for JSON strings right now as the other JSON types aren't implemented
    for (int i = 0; i < 100; ++i) {
        char *entry_key = (char *)malloc(sizeof(char) * 10);

        struct json_value *entry_val = (struct json_value *)malloc(sizeof(struct json_value));
        entry_val->child_object = NULL;
        entry_val->str_val = (char *)malloc(sizeof(char) * 5);
        entry_val->int_val = i;
        entry_val->float_val = i;
        entry_val->type = JSON_STRING;

        sprintf(entry_key, "entry%d", i);
        entry_key[9] = '\0';
        sprintf(entry_val->str_val, "%d", i);
        entry_val->str_val[4] = '\0';

        int res = json_add_entry(test_object, entry_key, entry_val);

        if (res) {
            printf("[TEST FAIL] test_json_add_entry(): entry num %d was null\n", i);

            return 1;
        }

        if (test_object->num_keys != (uint64_t)(i + 1)) {
            printf("[TEST FAIL] test_json_add_entry(): test_object->num_keys %ld does not equal expected value (%d)\n", test_object->num_keys, i + 1);

            return 1;
        }

        if (test_object->num_vals != (uint64_t)(i + 1)) {
            printf("[TEST FAIL] test_json_add_entry(): test_object->num_vals %ld does not equal expected value (%d)\n", test_object->num_vals, i + 1);

            return 1;
        }

        expected_str_size += strlen(entry_key) + 3;
        expected_str_size += strlen(entry_val->str_val) + 2;
        expected_str_size += 3;

        if (test_object->string_size != expected_str_size) {
            printf("[TEST FAIL] test_json_add_entry(): test_object->string_size %ld does not equal expected value (%ld)\n", test_object->string_size, expected_str_size);

            return 1;
        }
    }

    for (int i = 0; i < 100; i++) {
        char *entry_key = (char *)malloc(sizeof(char) * 10);
        char *entry_val = (char *)malloc(sizeof(char) * 5);

        sprintf(entry_key, "entry%d", i);
        entry_key[9] = '\0';
        sprintf(entry_val, "%d", i);
        entry_val[4] = '\0';

        if (test_object->vals[i]->type != JSON_STRING) {
            printf("[TEST FAIL] test_json_add_entry(): test_object->vals[%d]->type %d does not equal expected value (%d)\n", i, test_object->vals[i]->type, JSON_STRING);

            return 1;
        }

        if (strcmp(test_object->keys[i], entry_key)) {
            printf("[TEST FAIL] test_json_add_entry(): test_object->keys[%d] %s does not equal expected value (%s)\n", i, test_object->keys[i], entry_key);

            return 1;
        }

        if (strcmp(test_object->vals[i]->str_val, entry_val)) {
            printf("[TEST FAIL] test_json_add_entry(): test_object->vals[%d]->str_val %s does not equal expected value (%s)\n", i, test_object->vals[i]->str_val, entry_val);

            return 1;
        }

        free(entry_key);
        free(entry_val);
    }

    // TODO: Clean up JSON object

    return 0;
}

static int test_json_find_entry() {
    struct json_object *test_object = json_initialize_object();

    // TODO: Only testing for JSON strings right now as the other JSON types aren't implemented
    for (int i = 0; i < 100; ++i) {
        char *entry_key = (char *)malloc(sizeof(char) * 10);

        struct json_value *entry_val = (struct json_value *)malloc(sizeof(struct json_value));
        entry_val->child_object = NULL;
        entry_val->str_val = (char *)malloc(sizeof(char) * 5);
        entry_val->int_val = i;
        entry_val->float_val = i;
        entry_val->type = JSON_STRING;

        sprintf(entry_key, "entry%d", i);
        entry_key[9] = '\0';
        sprintf(entry_val->str_val, "%d", i);
        entry_val->str_val[4] = '\0';

        json_add_entry(test_object, entry_key, entry_val);
    }

    for (int i = 0; i < 100; i += 2) {
        char *entry_key = (char *)malloc(sizeof(char) * 10);
        char *entry_val = (char *)malloc(sizeof(char) * 5);

        sprintf(entry_key, "entry%d", i);
        entry_key[9] = '\0';
        sprintf(entry_val, "%d", i);
        entry_val[4] = '\0';

        struct json_value *found_entry = json_find_entry(test_object, entry_key);

        if (!found_entry) {
            printf("[TEST FAIL] test_json_find_entry(): could not find entry with key %s\n", entry_key);

            return 1;
        }

        if (strcmp(found_entry->str_val, entry_val)) {
            printf("[TEST FAIL] test_json_find_entry(): found_entry->str_val %s does not equal expected value (%s)\n", found_entry->str_val, entry_val);

            return 1;
        }

        if (found_entry->type != JSON_STRING) {
            printf("[TEST FAIL] test_json_find_entry(): found_entry->type %d does not equal expected value (%d)\n", found_entry->type, JSON_STRING);

            return 1;
        }

        free(entry_key);
        free(entry_val);
    }

    for (int i = 100; i < 200; ++i) {
        char *entry_key = (char *)malloc(sizeof(char) * 10);
        char *entry_val = (char *)malloc(sizeof(char) * 5);

        sprintf(entry_key, "entry%d", i);
        entry_key[9] = '\0';
        sprintf(entry_val, "%d", i);
        entry_val[4] = '\0';

        struct json_value *found_entry = json_find_entry(test_object, entry_key);

        if (found_entry) {
            printf("[TEST FAIL] test_json_find_entry(): found entry which should not exist (%s)\n", entry_key);

            return 1;
        }

        free(entry_key);
        free(entry_val);
    }

    return 0;
}

static int test_json_parse_string() {
    // TODO: More than just one test string
    char *test_string_one = "{\"0\":\"1\",\"1\":\"2\",\"2\":\"3\"}";

    struct json_object *test_object = json_parse_string(test_string_one);

    if (strlen(json_to_string(test_object)) != strlen(test_string_one)) {
        printf("[TEST FAIL] test_json_parse_string(): test_object->string_size %ld does not equal expected value (%ld)\n", test_object->string_size, strlen(test_string_one));

        return 1;
    }

    if (test_object->num_keys != 3) {
        printf("[TEST FAIL] test_json_parse_string(): test_object->num_keys %ld does not equal expected value (%d)\n", test_object->num_keys, 3);

        return 1;
    }

    if (test_object->num_vals != 3) {
        printf("[TEST FAIL] test_json_parse_string(): test_object->num_vals %ld does not equal expected value (%d)\n", test_object->num_vals, 3);

        return 1;
    }

    for (int i = 0; i < 3; ++i) {
        char *entry_key = (char *)malloc(sizeof(char) * 13);
        char *entry_val = (char *)malloc(sizeof(char) * 13);

        sprintf(entry_key, "%d", i);
        entry_key[12] = '\0';
        sprintf(entry_val, "%d", i + 1);
        entry_val[12] = '\0';

        if (strcmp(test_object->keys[i], entry_key)) {
            printf("[TEST FAIL] test_json_parse_string(): test_object->keys[%d] %s does not equal expected value (%s)\n", i, test_object->keys[i], entry_key);

            return 1;
        }

        if (strcmp(test_object->vals[i]->str_val, entry_val)) {
            printf("[TEST FAIL] test_json_parse_string(): test_object->vals[%d]->str_val %s does not equal expected value (%s)\n", i, test_object->vals[i]->str_val, entry_val);

            return 1;
        }

        free(entry_key);
        free(entry_val);
    }

    return 0;
}

static int test_json_to_string() {
    // TODO: More than just one test string
    char *test_string_one = "{\"0\":\"1\",\"1\":\"2\",\"2\":\"3\"}";

    struct json_object *test_object = json_initialize_object();

    for (int i = 0; i < 3; ++i) {
        char *entry_key = (char *)malloc(sizeof(char) * 13);
        struct json_value *entry_val = (struct json_value *)malloc(sizeof(struct json_value));
        entry_val->str_val = (char *)malloc(sizeof(char) * 13);

        sprintf(entry_key, "%d", i);
        entry_key[12] = '\0';

        sprintf(entry_val->str_val, "%d", i + 1);
        entry_val->str_val[12] = '\0';

        entry_val->type = JSON_STRING;

        json_add_entry(test_object, entry_key, entry_val);
    }

    char *new_string = json_to_string(test_object);

    if (strcmp(new_string, test_string_one)) {
        printf("[TEST FAIL] test_json_to_string(): new_string %s does not equal expected value (%s)\n", new_string, test_string_one);

        return 1;
    }

    return 0;
}

int test_json() {
    printf("Testing JSON handler\n");

    int failed_test_counter = 0;
    int num_tests = 0;

    failed_test_counter += test_json_initialize_object();
    ++num_tests;

    failed_test_counter += test_json_add_entry();
    ++num_tests;

    failed_test_counter += test_json_find_entry();
    ++num_tests;

    failed_test_counter += test_json_parse_string();
    ++num_tests;

    failed_test_counter += test_json_to_string();
    ++num_tests;

    printf("[TESTING COMPLETE] JSON testing %d/%d tests passed\n", num_tests - failed_test_counter, num_tests);

    return 0;
}
