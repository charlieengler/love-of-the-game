#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tests.h"

#include "../include/server/database/database.h"
#include "../include/server/utils/json.h"

// TODO: Implement me
static int test_db_initialize_no_file() {
    char *test_name = "test-initialize-no-file";

    struct json_value *db = db_initialize(test_name);

    char *filename = (char *)malloc(strlen("./databases/") + strlen(test_name) + 4);
    strcpy(filename, "./databases/");
    strcat(filename, test_name);
    strcat(filename, ".db\0");

    remove(filename);

    return 0;
}

// TODO: Implement me
static int test_db_initialize_with_file() {
    char *test_name = "test-initialize-with-file";

    struct json_value *db_json = db_initialize(test_name);
    if (!db_json) {
        printf("[TEST FAIL] test_db_initialize_with_file(): db_json was null\n");

        return 1;
    }

    if (db_json->type != JSON_OBJECT) {
        printf("[TEST FAIL] test_db_initialize_with_file(): db_json type (%d) was not JSON_OBJECT (%d)\n", db_json->type, JSON_OBJECT);

        return 1;
    }

    struct json_object *db_obj = (struct json_object *)(db_json->data);

    struct json_value *object_tests_json = json_object_get_value(db_obj, "object-tests");
    if (!object_tests_json) {
        printf("[TEST FAIL] test_db_initialize_with_file(): object_tests_json was null\n");

        return 1;
    }

    if (object_tests_json->type != JSON_OBJECT) {
        printf("[TEST FAIL] test_db_initialize_with_file(): object_tests_json type (%d) was not JSON_OBJECT (%d)\n", object_tests_json->type, JSON_OBJECT);

        return 1;
    }

    struct json_object *object_tests_object = (struct json_object *)(object_tests_json->data);

    struct json_value *object_objects_json = json_object_get_value(object_tests_object, "object-objects");
    if (!object_objects_json) {
        printf("[TEST FAIL] test_db_initialize_with_file(): object_objects_json was null\n");

        return 1;
    }

    if (object_objects_json->type != JSON_OBJECT) {
        printf("[TEST FAIL] test_db_initialize_with_file(): object_objects_json type (%d) was not JSON_OBJECT (%d)\n", object_objects_json->type, JSON_OBJECT);

        return 1;
    }

    struct json_object *object_objects_object = (struct json_object *)(object_objects_json->data);

    for (int i = 0; i < 3; ++i) {
    }

    return 0;
}

// TODO: Implement me
static int test_db_save() {
    char *test_name = "test-save";

    struct json_value *db = db_initialize(test_name);

    for (int i = 0; i < 10; ++i) {
        char *tmp_key = (char *)malloc(sizeof(char) * 13);
        sprintf(tmp_key, "%d", i);
        tmp_key[12] = '\0';
    }

    // TODO: Error checking
    db_save(db, test_name);

    db = db_initialize(test_name);

    for (int i = 9; i >= 0; --i) {
        char *tmp_key = (char *)malloc(sizeof(char) * 13);
        sprintf(tmp_key, "%d", i);
        tmp_key[12] = '\0';
    }

    return 0;
}

int test_db() {
    printf("Testing database\n");

    int failed_test_counter = 0;
    int num_tests = 0;

    failed_test_counter += test_db_initialize_no_file();
    ++num_tests;

    failed_test_counter += test_db_initialize_with_file();
    ++num_tests;

    failed_test_counter += test_db_save();
    ++num_tests;

    printf("[TESTING COMPLETE] DB testing %d/%d tests passed\n", num_tests - failed_test_counter, num_tests);

    return 0;
}
