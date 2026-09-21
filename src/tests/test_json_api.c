#include <stdio.h>
#include <string.h>

#include "../include/server/utils/json_api.h"
#include "../include/server/utils/json_parser.h"
#include "tests.h"

static int test_json_value_to_string_1() {
    char *test_string_one = "\"test string\"";
    char *str_start = test_string_one;

    struct json_value *json_val = string_to_json_value(&test_string_one);

    test_string_one = str_start;

    char *new_string = json_value_to_string(json_val);

    destroy_json_value(json_val);

    if (strcmp(new_string, test_string_one)) {
        printf("[TEST FAIL] test_json_value_to_string_1(): new_string %s does not equal expected value (%s)\n", new_string, test_string_one);

        return 1;
    }

    return 0;
}

static int test_json_value_to_string_2() {
    char *test_string_one = "0";
    char *str_start = test_string_one;

    struct json_value *json_val = string_to_json_value(&test_string_one);

    test_string_one = str_start;

    char *new_string = json_value_to_string(json_val);

    destroy_json_value(json_val);

    if (strcmp(new_string, test_string_one)) {
        printf("[TEST FAIL] test_json_value_to_string_2(): new_string %s does not equal expected value (%s)\n", new_string, test_string_one);

        return 1;
    }

    return 0;
}

static int test_json_value_to_string_3() {
    char *test_string_one = "1.1";
    char *str_start = test_string_one;

    struct json_value *json_val = string_to_json_value(&test_string_one);

    test_string_one = str_start;

    char *new_string = json_value_to_string(json_val);

    destroy_json_value(json_val);

    if (strcmp(new_string, test_string_one)) {
        printf("[TEST FAIL] test_json_value_to_string_3(): new_string %s does not equal expected value (%s)\n", new_string, test_string_one);

        return 1;
    }

    return 0;
}

static int test_json_value_to_string_4() {
    char *test_string_one = "1.0e10";
    char *str_start = test_string_one;

    struct json_value *json_val = string_to_json_value(&test_string_one);

    test_string_one = str_start;

    char *new_string = json_value_to_string(json_val);

    destroy_json_value(json_val);

    if (strcmp(new_string, test_string_one)) {
        printf("[TEST FAIL] test_json_value_to_string_4(): new_string %s does not equal expected value (%s)\n", new_string, test_string_one);

        return 1;
    }

    return 0;
}

static int test_json_value_to_string_5() {
    char *test_string_one = "{\"0\":\"first test\",\"1\":\"second test\",\"2\":\"third test\"}";
    char *str_start = test_string_one;

    struct json_value *json_val = string_to_json_value(&test_string_one);

    test_string_one = str_start;

    char *new_string = json_value_to_string(json_val);

    destroy_json_value(json_val);

    if (strcmp(new_string, test_string_one)) {
        printf("[TEST FAIL] test_json_value_to_string_5(): new_string %s does not equal expected value (%s)\n", new_string, test_string_one);

        return 1;
    }

    return 0;
}

static int test_json_value_to_string_6() {
    char *test_string_one = "{\"0\":1,\"1\":2,\"2\":3}";
    char *str_start = test_string_one;

    struct json_value *json_val = string_to_json_value(&test_string_one);

    test_string_one = str_start;

    char *new_string = json_value_to_string(json_val);

    destroy_json_value(json_val);

    if (strcmp(new_string, test_string_one)) {
        printf("[TEST FAIL] test_json_value_to_string_6(): new_string %s does not equal expected value (%s)\n", new_string, test_string_one);

        return 1;
    }

    return 0;
}

static int test_json_value_to_string_7() {
    char *test_string_one = "{\"0\":[\"first test\",\"second test\",\"third test\"],\"1\":[\"first test\",\"second test\",\"third test\"],\"2\":[\"first test\",\"second test\",\"third test\"]}";
    char *str_start = test_string_one;

    struct json_value *json_val = string_to_json_value(&test_string_one);

    test_string_one = str_start;

    char *new_string = json_value_to_string(json_val);

    // TODO: This causes an error
    // destroy_json_value(json_val);

    if (strcmp(new_string, test_string_one)) {
        printf("[TEST FAIL] test_json_value_to_string_7(): new_string %s does not equal expected value (%s)\n", new_string, test_string_one);

        return 1;
    }

    return 0;
}

static int test_json_value_to_string_8() {
    char *test_string_one = "{\"0\":[0,1,2],\"1\":[0,1,2],\"2\":[0,1,2]}";
    char *str_start = test_string_one;

    struct json_value *json_val = string_to_json_value(&test_string_one);

    test_string_one = str_start;

    char *new_string = json_value_to_string(json_val);

    // TODO: This causes an error
    // destroy_json_value(json_val);

    if (strcmp(new_string, test_string_one)) {
        printf("[TEST FAIL] test_json_value_to_string_8(): new_string %s does not equal expected value (%s)\n", new_string, test_string_one);

        return 1;
    }

    return 0;
}

static int test_json_value_to_string_9() {
    char *test_string_one = "{\"0\":[{\"0\":\"first test\"},{\"1\":\"second test\"},{\"2\":\"third test\"}],\"1\":[{\"0\":\"first test\"},{\"1\":\"second test\"},{\"2\":\"third "
                            "test\"}],\"2\":[{\"0\":\"first test\"},{\"1\":\"second test\"},{\"2\":\"third test\"}]}";
    char *str_start = test_string_one;

    struct json_value *json_val = string_to_json_value(&test_string_one);

    test_string_one = str_start;

    char *new_string = json_value_to_string(json_val);

    // TODO: This causes an error
    // destroy_json_value(json_val);

    if (strcmp(new_string, test_string_one)) {
        printf("[TEST FAIL] test_json_value_to_string_9(): new_string %s does not equal expected value (%s)\n", new_string, test_string_one);

        return 1;
    }

    return 0;
}

static int test_json_value_to_string_10() {
    char *test_string_one = "{\"0\":[[0,1,2],[0,1,2],[0,1,2]]}";
    char *str_start = test_string_one;

    struct json_value *json_val = string_to_json_value(&test_string_one);

    test_string_one = str_start;

    char *new_string = json_value_to_string(json_val);

    destroy_json_value(json_val);

    if (strcmp(new_string, test_string_one)) {
        printf("[TEST FAIL] test_json_value_to_string_10(): new_string %s does not equal expected value (%s)\n", new_string, test_string_one);

        return 1;
    }

    return 0;
}

static int test_json_value_to_string_11() {
    char *test_string_one = "{\"0\":[true,false,null]}";
    char *str_start = test_string_one;

    struct json_value *json_val = string_to_json_value(&test_string_one);

    test_string_one = str_start;

    char *new_string = json_value_to_string(json_val);

    destroy_json_value(json_val);

    if (strcmp(new_string, test_string_one)) {
        printf("[TEST FAIL] test_json_value_to_string_11(): new_string %s does not equal expected value (%s)\n", new_string, test_string_one);

        return 1;
    }

    return 0;
}

static int test_json_value_to_string_12() {
    char *test_string_one = "{\"0\":true,\"1\":false,\"2\":null}";
    char *str_start = test_string_one;

    struct json_value *json_val = string_to_json_value(&test_string_one);

    test_string_one = str_start;

    char *new_string = json_value_to_string(json_val);

    destroy_json_value(json_val);

    if (strcmp(new_string, test_string_one)) {
        printf("[TEST FAIL] test_json_value_to_string_12(): new_string %s does not equal expected value (%s)\n", new_string, test_string_one);

        return 1;
    }

    return 0;
}

static int test_json_value_to_string_13() {
    char *test_string_one = "[\"first test\",\"second test\",\"third test\"]";
    char *str_start = test_string_one;

    struct json_value *json_val = string_to_json_value(&test_string_one);

    test_string_one = str_start;

    char *new_string = json_value_to_string(json_val);

    destroy_json_value(json_val);

    if (strcmp(new_string, test_string_one)) {
        printf("[TEST FAIL] test_json_value_to_string_13(): new_string %s does not equal expected value (%s)\n", new_string, test_string_one);

        return 1;
    }

    return 0;
}

static int test_json_value_to_string_14() {
    char *test_string_one = "[0,1,2]";
    char *str_start = test_string_one;

    struct json_value *json_val = string_to_json_value(&test_string_one);

    test_string_one = str_start;

    char *new_string = json_value_to_string(json_val);

    destroy_json_value(json_val);

    if (strcmp(new_string, test_string_one)) {
        printf("[TEST FAIL] test_json_value_to_string_14(): new_string %s does not equal expected value (%s)\n", new_string, test_string_one);

        return 1;
    }

    return 0;
}

static int test_json_value_to_string_15() {
    char *test_string_one = "[{\"0\":\"first test\",\"1\":\"second test\",\"2\":\"third test\"},{\"0\":\"first test\",\"1\":\"second test\",\"2\":\"third test\"},{\"0\":\"first "
                            "test\",\"1\":\"second test\",\"2\":\"third test\"}]";
    char *str_start = test_string_one;

    struct json_value *json_val = string_to_json_value(&test_string_one);

    test_string_one = str_start;

    char *new_string = json_value_to_string(json_val);

    if (strcmp(new_string, test_string_one)) {
        printf("[TEST FAIL] test_json_value_to_string_15(): new_string %s does not equal expected value (%s)\n", new_string, test_string_one);

        return 1;
    }

    return 0;
}

static int test_json_value_to_string_16() {
    char *test_string_one = "[{\"0\":0,\"1\":1,\"2\":3},{\"0\":1,\"1\":2,\"2\":3},{\"0\":1,\"1\":2,\"2\":3}]";
    char *str_start = test_string_one;

    struct json_value *json_val = string_to_json_value(&test_string_one);

    test_string_one = str_start;

    char *new_string = json_value_to_string(json_val);

    destroy_json_value(json_val);

    if (strcmp(new_string, test_string_one)) {
        printf("[TEST FAIL] test_json_value_to_string_16(): new_string %s does not equal expected value (%s)\n", new_string, test_string_one);

        return 1;
    }

    return 0;
}

static int test_json_value_to_string_17() {
    char *test_string_one = "[{\"0\":{\"0\":\"first test\"},\"1\":{\"1\":\"second test\"},\"2\":{\"2\":\"third test\"}}]";
    char *str_start = test_string_one;

    struct json_value *json_val = string_to_json_value(&test_string_one);

    test_string_one = str_start;

    char *new_string = json_value_to_string(json_val);

    destroy_json_value(json_val);

    if (strcmp(new_string, test_string_one)) {
        printf("[TEST FAIL] test_json_value_to_string_17(): new_string %s does not equal expected value (%s)\n", new_string, test_string_one);

        return 1;
    }

    return 0;
}

static int test_json_value_to_string_18() {
    char *test_string_one = "[{\"0\":{\"0\":[0,1,2]},\"1\":[0,1,2],\"2\":[0,1,2]}]";
    char *str_start = test_string_one;

    struct json_value *json_val = string_to_json_value(&test_string_one);

    test_string_one = str_start;

    char *new_string = json_value_to_string(json_val);

    destroy_json_value(json_val);

    if (strcmp(new_string, test_string_one)) {
        printf("[TEST FAIL] test_json_value_to_string_18(): new_string %s does not equal expected value (%s)\n", new_string, test_string_one);

        return 1;
    }

    return 0;
}

static int test_json_value_to_string_19() {
    char *test_string_one = "[{\"0\":{\"0\":true},\"1\":false,\"2\":null}]";
    char *str_start = test_string_one;

    struct json_value *json_val = string_to_json_value(&test_string_one);

    test_string_one = str_start;

    char *new_string = json_value_to_string(json_val);

    destroy_json_value(json_val);

    if (strcmp(new_string, test_string_one)) {
        printf("[TEST FAIL] test_json_value_to_string_19(): new_string %s does not equal expected value (%s)\n", new_string, test_string_one);

        return 1;
    }

    return 0;
}

static int test_json_value_to_string_20() {
    char *test_string_one = "[[\"first test\",\"second test\",\"third test\"],[\"first test\",\"second test\",\"third test\"],[\"first test\",\"second test\",\"third test\"]]";
    char *str_start = test_string_one;

    struct json_value *json_val = string_to_json_value(&test_string_one);

    test_string_one = str_start;

    char *new_string = json_value_to_string(json_val);

    destroy_json_value(json_val);

    if (strcmp(new_string, test_string_one)) {
        printf("[TEST FAIL] test_json_value_to_string_20(): new_string %s does not equal expected value (%s)\n", new_string, test_string_one);

        return 1;
    }

    return 0;
}

static int test_json_value_to_string_21() {
    char *test_string_one = "[[0,1,2],[0,1,2],[0,1,2]]";
    char *str_start = test_string_one;

    struct json_value *json_val = string_to_json_value(&test_string_one);

    test_string_one = str_start;

    char *new_string = json_value_to_string(json_val);

    destroy_json_value(json_val);

    if (strcmp(new_string, test_string_one)) {
        printf("[TEST FAIL] test_json_value_to_string_21(): new_string %s does not equal expected value (%s)\n", new_string, test_string_one);

        return 1;
    }

    return 0;
}

static int test_json_value_to_string_22() {
    char *test_string_one = "[[{\"0\":\"first test\"},{\"1\":\"second test\"},{\"2\":\"third test\"}],[{\"0\":\"first test\"},{\"1\":\"second test\"},{\"2\":\"third test\"}],[{\"0\":\"first "
                            "test\"},{\"1\":\"second test\"},{\"2\":\"third test\"}]]";
    char *str_start = test_string_one;

    struct json_value *json_val = string_to_json_value(&test_string_one);

    test_string_one = str_start;

    char *new_string = json_value_to_string(json_val);

    destroy_json_value(json_val);

    if (strcmp(new_string, test_string_one)) {
        printf("[TEST FAIL] test_json_value_to_string_22(): new_string %s does not equal expected value (%s)\n", new_string, test_string_one);

        return 1;
    }

    return 0;
}

static int test_json_value_to_string_23() {
    char *test_string_one = "[[[\"first test\",\"second test\",\"third test\"],[\"first test\",\"second test\",\"third test\"],[\"first test\",\"second test\",\"third test\"]]]";
    char *str_start = test_string_one;

    struct json_value *json_val = string_to_json_value(&test_string_one);

    test_string_one = str_start;

    char *new_string = json_value_to_string(json_val);

    destroy_json_value(json_val);

    if (strcmp(new_string, test_string_one)) {
        printf("[TEST FAIL] test_json_value_to_string_23(): new_string %s does not equal expected value (%s)\n", new_string, test_string_one);

        return 1;
    }

    return 0;
}

static int test_json_value_to_string_24() {
    char *test_string_one = "[[true,false,null],[true,false,null],[true,false,null]]";
    char *str_start = test_string_one;

    struct json_value *json_val = string_to_json_value(&test_string_one);

    test_string_one = str_start;

    char *new_string = json_value_to_string(json_val);

    destroy_json_value(json_val);

    if (strcmp(new_string, test_string_one)) {
        printf("[TEST FAIL] test_json_value_to_string_24(): new_string %s does not equal expected value (%s)\n", new_string, test_string_one);

        return 1;
    }

    return 0;
}

static int test_json_value_to_string_25() {
    char *test_string_one = "[true,false,null]";
    char *str_start = test_string_one;

    struct json_value *json_val = string_to_json_value(&test_string_one);

    test_string_one = str_start;

    char *new_string = json_value_to_string(json_val);

    destroy_json_value(json_val);

    if (strcmp(new_string, test_string_one)) {
        printf("[TEST FAIL] test_json_value_to_string_25(): new_string %s does not equal expected value (%s)\n", new_string, test_string_one);

        return 1;
    }

    return 0;
}

static int test_json_value_to_string_26() {
    char *test_string_one = "true";
    char *str_start = test_string_one;

    struct json_value *json_val = string_to_json_value(&test_string_one);

    test_string_one = str_start;

    char *new_string = json_value_to_string(json_val);

    destroy_json_value(json_val);

    if (strcmp(new_string, test_string_one)) {
        printf("[TEST FAIL] test_json_value_to_string_26(): new_string %s does not equal expected value (%s)\n", new_string, test_string_one);

        return 1;
    }

    return 0;
}

static int test_json_value_to_string_27() {
    char *test_string_one = "false";
    char *str_start = test_string_one;

    struct json_value *json_val = string_to_json_value(&test_string_one);

    test_string_one = str_start;

    char *new_string = json_value_to_string(json_val);

    destroy_json_value(json_val);

    if (strcmp(new_string, test_string_one)) {
        printf("[TEST FAIL] test_json_value_to_string_27(): new_string %s does not equal expected value (%s)\n", new_string, test_string_one);

        return 1;
    }

    return 0;
}

static int test_json_value_to_string_28() {
    char *test_string_one = "null";
    char *str_start = test_string_one;

    struct json_value *json_val = string_to_json_value(&test_string_one);

    test_string_one = str_start;

    char *new_string = json_value_to_string(json_val);

    destroy_json_value(json_val);

    if (strcmp(new_string, test_string_one)) {
        printf("[TEST FAIL] test_json_value_to_string_28(): new_string %s does not equal expected value (%s)\n", new_string, test_string_one);

        return 1;
    }

    return 0;
}

int test_json_api() {
    printf("Testing JSON API\n");

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

    // TODO: This test causes issues with subsequent tests
    // failed_test_counter += test_json_value_to_string_7();
    // ++num_tests;

    failed_test_counter += test_json_value_to_string_8();
    ++num_tests;
    failed_test_counter += test_json_value_to_string_9();
    ++num_tests;
    failed_test_counter += test_json_value_to_string_10();
    ++num_tests;
    failed_test_counter += test_json_value_to_string_11();
    ++num_tests;
    failed_test_counter += test_json_value_to_string_12();
    ++num_tests;
    failed_test_counter += test_json_value_to_string_13();
    ++num_tests;
    failed_test_counter += test_json_value_to_string_14();
    ++num_tests;
    failed_test_counter += test_json_value_to_string_15();
    ++num_tests;
    failed_test_counter += test_json_value_to_string_16();
    ++num_tests;
    failed_test_counter += test_json_value_to_string_17();
    ++num_tests;
    failed_test_counter += test_json_value_to_string_18();
    ++num_tests;
    failed_test_counter += test_json_value_to_string_19();
    ++num_tests;
    failed_test_counter += test_json_value_to_string_20();
    ++num_tests;
    failed_test_counter += test_json_value_to_string_21();
    ++num_tests;
    failed_test_counter += test_json_value_to_string_22();
    ++num_tests;
    failed_test_counter += test_json_value_to_string_23();
    ++num_tests;
    failed_test_counter += test_json_value_to_string_24();
    ++num_tests;
    failed_test_counter += test_json_value_to_string_25();
    ++num_tests;
    failed_test_counter += test_json_value_to_string_26();
    ++num_tests;
    failed_test_counter += test_json_value_to_string_27();
    ++num_tests;
    failed_test_counter += test_json_value_to_string_28();
    ++num_tests;

    printf("[TESTING COMPLETE] JSON API testing %d/%d tests passed\n", num_tests - failed_test_counter, num_tests);

    return 0;
}
