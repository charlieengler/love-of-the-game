#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/server/utils/json.h"

#include "tests.h"

static int test_json_value_to_string_1() {
    printf("    Running test_json_value_to_string_1()\n");

    char *test_string = "{\n"
                        "    \"0\": \"1\",\n"
                        "    \"1\": \"2\",\n"
                        "    \"2\": \"3\"\n"
                        "}";
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

        char *child_str;
        struct json_value *child_value = json_object_get_value((struct json_object *)test_val->data, entry_key, (void **)&child_str, JSON_STRING);

        if (child_value->type != JSON_STRING) {
            printf("[TEST FAIL] test_json_parse_string_1(): child_value->type %d does not equal JSON_STRING\n", test_val->type);

            return 1;
        }

        if (strcmp(child_str, entry_val)) {
            printf("[TEST FAIL] test_json_parse_string_1(): child_value->data %s does not equal expected value (%s)\n", child_str, entry_val);

            return 1;
        }

        free(entry_key);
        free(entry_val);
    }

    return 0;
}

static int test_json_value_to_string_2() {
    printf("    Running test_json_value_to_string_2()\n");

    char *test_string = "{\n"
                        "    \"0\": 1,\n"
                        "    \"1\": 2,\n"
                        "    \"2\": 3\n"
                        "}";
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

        struct json_number *child_num;
        struct json_value *child_value = json_object_get_value((struct json_object *)test_val->data, entry_key, (void **)&child_num, JSON_NUMBER);

        if (child_value->type != JSON_NUMBER) {
            printf("[TEST FAIL] test_json_parse_string_2(): child_value->type %d does not equal JSON_NUMBER\n", test_val->type);

            return 1;
        }

        if (child_num->type != JSON_INTEGER) {
            printf("[TEST FAIL] test_json_parse_string_2(): child_value->data->type %d does not equal JSON_INTEGER\n", child_num->type);

            return 1;
        }

        if (child_num->integer != i + 1) {
            printf("[TEST FAIL] test_json_parse_string_2(): child_value->data %lld does not equal expected value (%d)\n", child_num->integer, i + 1);

            return 1;
        }

        free(entry_key);
    }

    return 0;
}

static int test_json_value_to_string_3() {
    printf("    Running test_json_value_to_string_3()\n");

    char *test_string = "{\n"
                        "    \"0\": {\n"
                        "        \"1\": 2\n"
                        "    },\n"
                        "    \"1\": {\n"
                        "        \"2\": 3\n"
                        "    },\n"
                        "    \"2\": {\n"
                        "        \"3\": 4\n"
                        "    }\n"
                        "}";
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

        struct json_object *child_object;
        struct json_value *child_value = json_object_get_value((struct json_object *)test_val->data, entry_key, (void **)&child_object, JSON_OBJECT);

        if (child_value->type != JSON_OBJECT) {
            printf("[TEST FAIL] test_json_parse_string_3(): child_value->type %d does not equal JSON_OBJECT\n", test_val->type);

            return 1;
        }

        char *entry_entry_key = (char *)calloc(13, sizeof(char));

        sprintf(entry_entry_key, "%d", i + 1);

        struct json_number *child_child_num;
        struct json_value *child_child_value = json_object_get_value(child_object, entry_entry_key, (void **)&child_child_num, JSON_NUMBER);
        if (child_child_value->type != JSON_NUMBER) {
            printf("[TEST FAIL] test_json_parse_string_3(): child_child_value->type %d does not equal JSON_NUMBER\n", child_child_value->type);

            return 1;
        }

        if (child_child_num->type != JSON_INTEGER) {
            printf("[TEST FAIL] test_json_parse_string_3(): child_child_value->data->type %d does not equal JSON_INTEGER\n", child_child_num->type);

            return 1;
        }

        if (child_child_num->integer != i + 2) {
            printf("[TEST FAIL] test_json_parse_string_3(): child_child_value->data->integer %lld does not equal expected value (%d)\n", child_child_num->integer, i + 2);

            return 1;
        }

        free(entry_entry_key);

        free(entry_key);
    }

    return 0;
}

static int test_json_value_to_string_4() {
    printf("    Running test_json_value_to_string_4()\n");

    char *test_string = "{\n"
                        "    \"0\": [\n"
                        "        0,\n"
                        "        1,\n"
                        "        2\n"
                        "    ],\n"
                        "    \"1\": [\n"
                        "        0,\n"
                        "        1,\n"
                        "        2\n"
                        "    ],\n"
                        "    \"2\": [\n"
                        "        0,\n"
                        "        1,\n"
                        "        2\n"
                        "    ]\n"
                        "}";
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

        struct json_array *child_arr;
        struct json_value *child_value = json_object_get_value((struct json_object *)test_val->data, entry_key, (void **)&child_arr, JSON_ARRAY);

        if (child_value->type != JSON_ARRAY) {
            printf("[TEST FAIL] test_json_parse_string_4(): child_value->type %d does not equal JSON_ARRAY\n", test_val->type);

            return 1;
        }

        for (int j = 0; j < 3; ++j) {
            struct json_value *child_child_value = child_arr->values[j];
            struct json_number *child_child_num = (struct json_number *)child_child_value->data;
            if (child_child_value->type != JSON_NUMBER) {
                printf("[TEST FAIL] test_json_parse_string_4(): child_child_value->type %d does not equal JSON_NUMBER\n", child_child_num->type);

                return 1;
            }

            if (child_child_num->type != JSON_INTEGER) {
                printf("[TEST FAIL] test_json_parse_string_4(): child_child_value->data->type %d does not equal JSON_INTEGER\n", child_child_num->type);

                return 1;
            }

            if (child_child_num->integer != j) {
                printf("[TEST FAIL] test_json_parse_string_4(): child_child_value->data->integer %lld does not equal expected value (%d)\n", child_child_num->integer, j);

                return 1;
            }
        }

        free(entry_key);
    }

    return 0;
}

static int test_json_value_to_string_5() {
    printf("    Running test_json_value_to_string_5()\n");

    char *test_string = "{\n"
                        "    \"0\": true,\n"
                        "    \"1\": true,\n"
                        "    \"2\": true\n"
                        "}";
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

        // TODO: Need to check if child_value == NULL, the same goes for the other tests
        struct json_value *child_value = json_object_get_value((struct json_object *)test_val->data, entry_key, NULL, JSON_TRUE);

        if (child_value->type != JSON_TRUE) {
            printf("[TEST FAIL] test_json_parse_string_5(): child_value->type %d does not equal JSON_TRUE\n", test_val->type);

            return 1;
        }

        free(entry_key);
    }

    return 0;
}

static int test_json_value_to_string_6() {
    printf("    Running test_json_value_to_string_6()\n");

    char *test_string = "{\n"
                        "    \"0\": false,\n"
                        "    \"1\": false,\n"
                        "    \"2\": false\n"
                        "}";
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

        struct json_value *child_value = json_object_get_value((struct json_object *)test_val->data, entry_key, NULL, JSON_FALSE);

        if (child_value->type != JSON_FALSE) {
            printf("[TEST FAIL] test_json_parse_string_6(): child_value->type %d does not equal JSON_FALSE\n", test_val->type);

            return 1;
        }

        free(entry_key);
    }

    return 0;
}

static int test_json_value_to_string_7() {
    printf("    Running test_json_value_to_string_7()\n");

    char *test_string = "{\n"
                        "    \"0\": null,\n"
                        "    \"1\": null,\n"
                        "    \"2\": null\n"
                        "}";
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

        struct json_value *child_value = json_object_get_value((struct json_object *)test_val->data, entry_key, NULL, JSON_NULL);

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
