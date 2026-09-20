#include <stdio.h>
#include <string.h>

#include "../include/server/utils/json_api.h"
#include "../include/server/utils/json_parser.h"
#include "tests.h"

static int test_json_value_to_string() {
    // TODO: More than just one test string
    char *test_string_one = "{\"0\":\"1\",\"1\":\"2\",\"2\":\"3\"}";

    struct json_value *json_val = string_to_json_value(&test_string_one);

    char *new_string = json_value_to_string(json_val);

    if (strcmp(new_string, test_string_one)) {
        printf("[TEST FAIL] test_json_value_to_string(): new_string %s does not equal expected value (%s)\n", new_string, test_string_one);

        return 1;
    }

    return 0;
}

int test_json_api() {
    printf("Testing JSON API\n");

    int failed_test_counter = 0;
    int num_tests = 0;

    failed_test_counter += test_json_value_to_string();
    ++num_tests;

    printf("[TESTING COMPLETE] JSON API testing %d/%d tests passed\n", num_tests - failed_test_counter, num_tests);

    return 0;
}
