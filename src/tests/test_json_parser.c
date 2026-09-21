#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/server/utils/json_api.h"
#include "../include/server/utils/json_parser.h"

#include "tests.h"

static int test_json_value_to_string_1() {
    char *test_string = "{\"0\":\"1\",\"1\":\"2\",\"2\":\"3\"}";
    char *str_start = test_string;

    struct json_value *test_val = string_to_json_value(&test_string);

    test_string = str_start;

    if (strcmp(json_value_to_string(test_val), test_string)) {
        printf("[TEST FAIL] test_json_value_to_string_1(): test_val string %s does not equal expected value (%s)\n", json_value_to_string(test_val), test_string);

        return 1;
    }

    for (int i = 0; i < 3; ++i) {
        char *entry_key = (char *)calloc(13, sizeof(char));
        char *entry_val = (char *)calloc(13, sizeof(char));

        sprintf(entry_key, "%d", i);
        sprintf(entry_val, "%d", i + 1);

        if (strcmp(((struct json_object *)test_val->data)->keys[i], entry_key)) {
            printf("[TEST FAIL] test_json_parse_string_1(): test_val->data->keys[%d] %s does not equal expected value (%s)\n", i, ((struct json_object *)test_val->data)->keys[i], entry_key);

            return 1;
        }

        struct json_value *child_value = json_object_get_value((struct json_object *)test_val->data, ((struct json_object *)test_val->data)->keys[i]);

        if (child_value->type != JSON_STRING) {
            printf("[TEST FAIL] test_json_parse_string_1(): child_value->type %d does not equal JSON_STRING\n", test_val->type);

            return 1;
        }

        if (strcmp((char *)child_value->data, entry_val)) {
            printf("[TEST FAIL] test_json_parse_string_1(): child_value->data %s does not equal expected value (%s)\n", (char *)child_value->data, entry_val);

            return 1;
        }

        free(entry_key);
        free(entry_val);
    }

    return 0;
}

static int test_json_value_to_string_2() {
    char *test_string = "{\"0\":1,\"1\":2,\"2\":3}";
    char *str_start = test_string;

    struct json_value *test_val = string_to_json_value(&test_string);

    test_string = str_start;

    if (strcmp(json_value_to_string(test_val), test_string)) {
        printf("[TEST FAIL] test_json_value_to_string_2(): test_val string %s does not equal expected value (%s)\n", json_value_to_string(test_val), test_string);

        return 1;
    }

    for (int i = 0; i < 3; ++i) {
        char *entry_key = (char *)calloc(13, sizeof(char));

        sprintf(entry_key, "%d", i);

        if (strcmp(((struct json_object *)test_val->data)->keys[i], entry_key)) {
            printf("[TEST FAIL] test_json_parse_string_2(): test_val->data->keys[%d] %s does not equal expected value (%s)\n", i, ((struct json_object *)test_val->data)->keys[i], entry_key);

            return 1;
        }

        struct json_value *child_value = json_object_get_value((struct json_object *)test_val->data, ((struct json_object *)test_val->data)->keys[i]);

        if (child_value->type != JSON_NUMBER) {
            printf("[TEST FAIL] test_json_parse_string_2(): child_value->type %d does not equal JSON_NUMBER\n", test_val->type);

            return 1;
        }

        if (((struct json_number *)child_value->data)->type != JSON_INTEGER) {
            printf("[TEST FAIL] test_json_parse_string_2(): child_value->data->type %d does not equal JSON_INTEGER\n", ((struct json_number *)child_value->data)->type);

            return 1;
        }

        if (((struct json_number *)child_value->data)->integer != i + 1) {
            printf("[TEST FAIL] test_json_parse_string_2(): child_value->data %lld does not equal expected value (%d)\n", ((struct json_number *)child_value->data)->integer, i + 1);

            return 1;
        }

        free(entry_key);
    }

    return 0;
}

static int test_json_value_to_string_3() {
    char *test_string = "{\"0\":{\"1\":2},\"1\":{\"2\":3},\"2\":{\"3\":4}}";
    char *str_start = test_string;

    struct json_value *test_val = string_to_json_value(&test_string);

    test_string = str_start;

    if (strcmp(json_value_to_string(test_val), test_string)) {
        printf("[TEST FAIL] test_json_value_to_string_3(): test_val string %s does not equal expected value (%s)\n", json_value_to_string(test_val), test_string);

        return 1;
    }

    for (int i = 0; i < 3; ++i) {
        char *entry_key = (char *)calloc(13, sizeof(char));

        sprintf(entry_key, "%d", i);

        if (strcmp(((struct json_object *)test_val->data)->keys[i], entry_key)) {
            printf("[TEST FAIL] test_json_parse_string_3(): test_val->data->keys[%d] %s does not equal expected value (%s)\n", i, ((struct json_object *)test_val->data)->keys[i], entry_key);

            return 1;
        }

        struct json_value *child_value = json_object_get_value((struct json_object *)test_val->data, ((struct json_object *)test_val->data)->keys[i]);

        if (child_value->type != JSON_OBJECT) {
            printf("[TEST FAIL] test_json_parse_string_3(): child_value->type %d does not equal JSON_OBJECT\n", test_val->type);

            return 1;
        }

        char *entry_entry_key = (char *)calloc(13, sizeof(char));

        sprintf(entry_entry_key, "%d", i + 1);

        if (strcmp(((struct json_object *)child_value->data)->keys[0], entry_entry_key)) {
            printf("[TEST FAIL] test_json_parse_string_3(): child_value->data->keys[%d] %s does not equal expected value (%s)\n", 0, ((struct json_object *)child_value->data)->keys[0],
                   entry_entry_key);

            return 1;
        }

        struct json_value *child_child_value = json_object_get_value((struct json_object *)child_value->data, ((struct json_object *)child_value->data)->keys[0]);
        if (child_child_value->type != JSON_NUMBER) {
            printf("[TEST FAIL] test_json_parse_string_3(): child_child_value->type %d does not equal JSON_NUMBER\n", ((struct json_number *)child_child_value->data)->type);

            return 1;
        }

        if (((struct json_number *)child_child_value->data)->type != JSON_INTEGER) {
            printf("[TEST FAIL] test_json_parse_string_3(): child_child_value->data->type %d does not equal JSON_INTEGER\n", ((struct json_number *)child_child_value->data)->type);

            return 1;
        }

        if (((struct json_number *)child_child_value->data)->integer != i + 2) {
            printf("[TEST FAIL] test_json_parse_string_3(): child_child_value->data->integer %lld does not equal expected value (%d)\n", ((struct json_number *)child_child_value->data)->integer,
                   i + 2);

            return 1;
        }

        free(entry_entry_key);

        free(entry_key);
    }

    return 0;
}

static int test_json_value_to_string_4() {
    char *test_string = "{\"0\":[0,1,2],\"1\":[0,1,2],\"2\":[0,1,2]}";
    char *str_start = test_string;

    struct json_value *test_val = string_to_json_value(&test_string);

    test_string = str_start;

    if (strcmp(json_value_to_string(test_val), test_string)) {
        printf("[TEST FAIL] test_json_value_to_string_4(): test_val string %s does not equal expected value (%s)\n", json_value_to_string(test_val), test_string);

        return 1;
    }

    for (int i = 0; i < 3; ++i) {
        char *entry_key = (char *)calloc(13, sizeof(char));

        sprintf(entry_key, "%d", i);

        if (strcmp(((struct json_object *)test_val->data)->keys[i], entry_key)) {
            printf("[TEST FAIL] test_json_parse_string_4(): test_val->data->keys[%d] %s does not equal expected value (%s)\n", i, ((struct json_object *)test_val->data)->keys[i], entry_key);

            return 1;
        }

        struct json_value *child_value = json_object_get_value((struct json_object *)test_val->data, ((struct json_object *)test_val->data)->keys[i]);

        if (child_value->type != JSON_ARRAY) {
            printf("[TEST FAIL] test_json_parse_string_4(): child_value->type %d does not equal JSON_ARRAY\n", test_val->type);

            return 1;
        }

        for (int j = 0; j < 3; ++j) {
            struct json_value *child_child_value = ((struct json_array *)child_value->data)->values[j];
            if (child_child_value->type != JSON_NUMBER) {
                printf("[TEST FAIL] test_json_parse_string_4(): child_child_value->type %d does not equal JSON_NUMBER\n", ((struct json_number *)child_child_value->data)->type);

                return 1;
            }

            if (((struct json_number *)child_child_value->data)->type != JSON_INTEGER) {
                printf("[TEST FAIL] test_json_parse_string_4(): child_child_value->data->type %d does not equal JSON_INTEGER\n", ((struct json_number *)child_child_value->data)->type);

                return 1;
            }

            if (((struct json_number *)child_child_value->data)->integer != j) {
                printf("[TEST FAIL] test_json_parse_string_4(): child_child_value->data->integer %lld does not equal expected value (%d)\n", ((struct json_number *)child_child_value->data)->integer,
                       j);

                return 1;
            }
        }

        free(entry_key);
    }

    return 0;
}

static int test_json_value_to_string_5() {
    char *test_string = "{\"0\":true,\"1\":true,\"2\":true}";
    char *str_start = test_string;

    struct json_value *test_val = string_to_json_value(&test_string);

    test_string = str_start;

    if (strcmp(json_value_to_string(test_val), test_string)) {
        printf("[TEST FAIL] test_json_value_to_string_5(): test_val string %s does not equal expected value (%s)\n", json_value_to_string(test_val), test_string);

        return 1;
    }

    for (int i = 0; i < 3; ++i) {
        char *entry_key = (char *)calloc(13, sizeof(char));

        sprintf(entry_key, "%d", i);

        if (strcmp(((struct json_object *)test_val->data)->keys[i], entry_key)) {
            printf("[TEST FAIL] test_json_parse_string_5(): test_val->data->keys[%d] %s does not equal expected value (%s)\n", i, ((struct json_object *)test_val->data)->keys[i], entry_key);

            return 1;
        }

        struct json_value *child_value = json_object_get_value((struct json_object *)test_val->data, ((struct json_object *)test_val->data)->keys[i]);

        if (child_value->type != JSON_TRUE) {
            printf("[TEST FAIL] test_json_parse_string_5(): child_value->type %d does not equal JSON_TRUE\n", test_val->type);

            return 1;
        }

        free(entry_key);
    }

    return 0;
}

static int test_json_value_to_string_6() {
    char *test_string = "{\"0\":false,\"1\":false,\"2\":false}";
    char *str_start = test_string;

    struct json_value *test_val = string_to_json_value(&test_string);

    test_string = str_start;

    if (strcmp(json_value_to_string(test_val), test_string)) {
        printf("[TEST FAIL] test_json_value_to_string_6(): test_val string %s does not equal expected value (%s)\n", json_value_to_string(test_val), test_string);

        return 1;
    }

    for (int i = 0; i < 3; ++i) {
        char *entry_key = (char *)calloc(13, sizeof(char));

        sprintf(entry_key, "%d", i);

        if (strcmp(((struct json_object *)test_val->data)->keys[i], entry_key)) {
            printf("[TEST FAIL] test_json_parse_string_6(): test_val->data->keys[%d] %s does not equal expected value (%s)\n", i, ((struct json_object *)test_val->data)->keys[i], entry_key);

            return 1;
        }

        struct json_value *child_value = json_object_get_value((struct json_object *)test_val->data, ((struct json_object *)test_val->data)->keys[i]);

        if (child_value->type != JSON_FALSE) {
            printf("[TEST FAIL] test_json_parse_string_6(): child_value->type %d does not equal JSON_FALSE\n", test_val->type);

            return 1;
        }

        free(entry_key);
    }

    return 0;
}

static int test_json_value_to_string_7() {
    char *test_string = "{\"0\":null,\"1\":null,\"2\":null}";
    char *str_start = test_string;

    struct json_value *test_val = string_to_json_value(&test_string);

    test_string = str_start;

    if (strcmp(json_value_to_string(test_val), test_string)) {
        printf("[TEST FAIL] test_json_value_to_string_7(): test_val string %s does not equal expected value (%s)\n", json_value_to_string(test_val), test_string);

        return 1;
    }

    for (int i = 0; i < 3; ++i) {
        char *entry_key = (char *)calloc(13, sizeof(char));

        sprintf(entry_key, "%d", i);

        if (strcmp(((struct json_object *)test_val->data)->keys[i], entry_key)) {
            printf("[TEST FAIL] test_json_parse_string_7(): test_val->data->keys[%d] %s does not equal expected value (%s)\n", i, ((struct json_object *)test_val->data)->keys[i], entry_key);

            return 1;
        }

        struct json_value *child_value = json_object_get_value((struct json_object *)test_val->data, ((struct json_object *)test_val->data)->keys[i]);

        if (child_value->type != JSON_NULL) {
            printf("[TEST FAIL] test_json_parse_string_7(): child_value->type %d does not equal JSON_NULL\n", test_val->type);

            return 1;
        }

        free(entry_key);
    }

    return 0;
}

int test_json_parser() {
    printf("Testing JSON parser\n");

    int failed_test_counter = 0;
    int num_tests = 0;

    failed_test_counter += test_json_value_to_string_1();
    ++num_tests;

    failed_test_counter += test_json_value_to_string_2();
    ++num_tests;

    failed_test_counter += test_json_value_to_string_3();
    ++num_tests;

    failed_test_counter += test_json_value_to_string_4();
    ++num_tests;

    failed_test_counter += test_json_value_to_string_5();
    ++num_tests;

    failed_test_counter += test_json_value_to_string_6();
    ++num_tests;

    failed_test_counter += test_json_value_to_string_7();
    ++num_tests;

    printf("[TESTING COMPLETE] JSON parser testing %d/%d tests passed\n", num_tests - failed_test_counter, num_tests);

    return 0;
}
