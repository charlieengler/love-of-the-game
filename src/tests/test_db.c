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

    struct json_value *db = db_initialize(test_name);

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
