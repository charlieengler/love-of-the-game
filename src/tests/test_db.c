#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tests.h"

#include "../include/server/database/database.h"
#include "../include/server/utils/json_handler.h"

static int test_db_initialize_no_file() {
    struct database_mappings *mappings;
    const char *test_name = "test-initialize-no-file";

    uint64_t num_entries = db_initialize(&mappings, "test-initialize-no-file");

    if (num_entries != 0) {
        printf("[TEST FAIL] test_db_initialize_no_file(): database initialization entry count %ld does not equal expected value (%d)\n", num_entries, 0);

        return 1;
    }

    if (!mappings) {
        printf("[TEST FAIL] test_db_initialize_no_file(): mappings was null\n");

        return 1;
    }

    if (mappings->num_keys != 0) {
        printf("[TEST FAIL] test_db_initialize_no_file(): mappings->num_keys %ld does not equal expected value (0) \n", mappings->num_keys);

        return 1;
    }

    if (mappings->num_entries != 0) {
        printf("[TEST FAIL] test_db_initialize_no_file(): mappings->num_entries %ld does not equal expected value (0)\n", mappings->num_entries);

        return 1;
    }

    if (mappings->num_allocated != DB_NUM_DEFAULT_ENTRIES) {
        printf("[TEST FAIL] test_db_initialize_no_file(): mappings->num_allocated %ld does not equal expected value (%d)\n", mappings->num_allocated, DB_NUM_DEFAULT_ENTRIES);

        return 1;
    }

    if (strcmp(mappings->db_name, test_name)) {
        printf("[TEST FAIL] test_db_initialize_no_file(): mappings->db_name %s does not equal expected value (%s)\n", mappings->db_name, test_name);

        return 1;
    }

    if (!mappings->keys) {
        printf("[TEST FAIL] test_db_initialize_no_file(): mappings->keys was null\n");

        return 1;
    }

    if (!mappings->entries) {
        printf("[TEST FAIL] test_db_initialize_no_file(): mappings->entries was null\n");

        return 1;
    }

    db_close(mappings);

    char *filename = (char *)malloc(strlen("./databases/") + strlen(test_name) + 4);
    strcpy(filename, "./databases/");
    strcat(filename, test_name);
    strcat(filename, ".db\0");

    remove(filename);

    return 0;
}

static int test_db_initialize_with_file() {
    struct database_mappings *mappings;
    const char *test_name = "test-initialize-with-file";

    uint64_t num_entries = db_initialize(&mappings, "test-initialize-with-file");

    if (num_entries != 10) {
        printf("[TEST FAIL] test_db_initialize_with_file(): database initialization entry count %ld does not equal expected value (%d)\n", num_entries, 10);

        return 1;
    }

    if (!mappings) {
        printf("[TEST FAIL] test_db_initialize_with_file(): mappings was null\n");

        return 1;
    }

    if (mappings->num_keys != 10) {
        printf("[TEST FAIL] test_db_initialize_with_file(): mappings->num_keys %ld does not equal expected value (10) \n", mappings->num_keys);

        return 1;
    }

    if (mappings->num_entries != 10) {
        printf("[TEST FAIL] test_db_initialize_with_file(): mappings->num_entries %ld does not equal expected value (10)\n", mappings->num_entries);

        return 1;
    }

    if (mappings->num_allocated != 512) {
        printf("[TEST FAIL] test_db_initialize_with_file(): mappings->num_allocated %ld does not equal expected value (%d)\n", mappings->num_allocated, 512);

        return 1;
    }

    if (strcmp(mappings->db_name, test_name)) {
        printf("[TEST FAIL] test_db_initialize_with_file(): mappings->db_name %s does not equal expected value (%s)\n", mappings->db_name, test_name);

        return 1;
    }

    if (!mappings->keys) {
        printf("[TEST FAIL] test_db_initialize_with_file(): mappings->keys was null\n");

        return 1;
    }

    if (!mappings->entries) {
        printf("[TEST FAIL] test_db_initialize_with_file(): mappings->entries was null\n");

        return 1;
    }

    for (int i = 0; i < 10; ++i) {
        char *tmp_key = (char *)malloc(sizeof(char) * 13);
        sprintf(tmp_key, "%d", i);
        tmp_key[12] = '\0';

        struct database_entry *tmp_entry = db_find(mappings, tmp_key);

        if (!tmp_entry) {
            printf("[TEST FAIL] test_db_initialize_with_file(): tmp_entry was null at key %s\n", tmp_key);

            return 1;
        }

        if (strcmp(tmp_entry->key, tmp_key)) {
            printf("[TEST FAIL] test_db_initialize_with_file(): tmp_entry->key %s does not equal expected value (%s)\n", tmp_entry->key, tmp_key);

            return 1;
        }

        if (tmp_entry->type != DB_STRING) {
            printf("[TEST FAIL] test_db_initialize_with_file(): tmp_entry->type %d does not equal expected value (%d)\n", tmp_entry->type, DB_STRING);

            return 1;
        }

        struct json_object *json = json_parse_string((char *)tmp_entry->data_ptr);

        if (strcmp(json->vals[0]->str_val, tmp_key)) {
            printf("[TEST FAIL] test_db_initialize_with_file(): tmp_entry json data value %s does not equal expected value (%s)\n", json->vals[0]->str_val, tmp_key);

            return 1;
        }

        // TODO: Destroy the json object
    }

    db_close(mappings);

    return 0;
}

static int test_db_find() {
    struct database_mappings *mappings;

    db_initialize(&mappings, "test-initialize-with-file");

    for (int i = 9; i >= 0; --i) {
        char *tmp_key = (char *)malloc(sizeof(char) * 13);
        sprintf(tmp_key, "%d", i);
        tmp_key[12] = '\0';

        struct database_entry *tmp_entry = db_find(mappings, tmp_key);

        if (!tmp_entry) {
            printf("[TEST FAIL] test_db_find(): tmp_entry was null at key %s\n", tmp_key);

            return 1;
        }

        if (strcmp(tmp_entry->key, tmp_key)) {
            printf("[TEST FAIL] test_db_find(): tmp_entry->key %s does not equal expected value (%s)\n", tmp_entry->key, tmp_key);

            return 1;
        }

        if (tmp_entry->type != DB_STRING) {
            printf("[TEST FAIL] test_db_find(): tmp_entry->type %d does not equal expected value (%d)\n", tmp_entry->type, DB_STRING);

            return 1;
        }

        struct json_object *json = json_parse_string((char *)tmp_entry->data_ptr);

        if (strcmp(json->vals[0]->str_val, tmp_key)) {
            printf("[TEST FAIL] test_db_find(): tmp_entry json data value %s does not equal expected value (%s)\n", json->vals[0]->str_val, tmp_key);

            return 1;
        }

        // TODO: Destroy the json object
    }

    char *tmp_key = (char *)malloc(sizeof(char) * 13);
    sprintf(tmp_key, "%d", 10);
    tmp_key[12] = '\0';

    struct database_entry *tmp_entry = db_find(mappings, tmp_key);

    if (tmp_entry) {
        printf("[TEST FAIL] test_db_find(): found a non-existent entry at key %s\n", tmp_key);

        return 1;
    }

    free(tmp_key);

    db_close(mappings);

    return 0;
}

static int test_db_insert() {
    struct database_mappings *mappings;

    db_initialize(&mappings, "test-insert");

    for (int i = 0; i < 10; ++i) {
        char *tmp_key = (char *)malloc(sizeof(char) * 13);
        sprintf(tmp_key, "%d", i);
        tmp_key[12] = '\0';

        struct database_entry *tmp_entry = malloc(sizeof(struct database_entry));
        tmp_entry->key = tmp_key;
        tmp_entry->type = DB_STRING;

        struct json_object *json = malloc(sizeof(struct json_object));
        json->keys = malloc(sizeof(char *));
        json->keys[0] = tmp_key;
        json->vals = malloc(sizeof(struct json_value *));

        struct json_value *json_val = malloc(sizeof(struct json_value));
        json_val->child_object = NULL;
        json_val->str_val = tmp_key;
        json_val->type = JSON_STRING;

        json->vals[0] = json_val;
        json->num_keys = 1;
        json->num_vals = 1;
        json->string_size = strlen("{\"\":\"\"}") + strlen(tmp_key) * 2 + 1;

        tmp_entry->data_ptr = json_to_string(json);

        // TODO: Destroy the json object

        db_insert(mappings, tmp_entry);
    }

    for (int i = 9; i >= 0; --i) {
        char *tmp_key = (char *)malloc(sizeof(char) * 13);
        sprintf(tmp_key, "%d", i);
        tmp_key[12] = '\0';

        struct database_entry *tmp_entry = db_find(mappings, tmp_key);

        if (!tmp_entry) {
            printf("[TEST FAIL] test_db_insert(): tmp_entry was null at key %s\n", tmp_key);

            return 1;
        }

        if (strcmp(tmp_entry->key, tmp_key)) {
            printf("[TEST FAIL] test_db_insert(): tmp_entry->key %s does not equal expected value (%s)\n", tmp_entry->key, tmp_key);

            return 1;
        }

        if (tmp_entry->type != DB_STRING) {
            printf("[TEST FAIL] test_db_insert(): tmp_entry->type %d does not equal expected value (%d)\n", tmp_entry->type, DB_STRING);

            return 1;
        }

        struct json_object *json = json_parse_string((char *)tmp_entry->data_ptr);

        if (strcmp(json->vals[0]->str_val, tmp_key)) {
            printf("[TEST FAIL] test_db_insert(): tmp_entry json data value %s does not equal expected value (%s)\n", json->vals[0]->str_val, tmp_key);

            return 1;
        }

        // TODO: Destroy the json object
    }

    char *tmp_key = (char *)malloc(sizeof(char) * 13);
    sprintf(tmp_key, "%d", 10);
    tmp_key[12] = '\0';

    struct database_entry *tmp_entry = db_find(mappings, tmp_key);

    if (tmp_entry) {
        printf("[TEST FAIL] test_db_insert(): found a non-existent entry at key %s\n", tmp_key);

        return 1;
    }

    free(tmp_key);

    db_close(mappings);

    return 0;
}

static int test_db_save() {
    struct database_mappings *mappings;
    const char *test_name = "test-save";

    db_initialize(&mappings, "test-save");

    for (int i = 0; i < 10; ++i) {
        char *tmp_key = (char *)malloc(sizeof(char) * 13);
        sprintf(tmp_key, "%d", i);
        tmp_key[12] = '\0';

        struct database_entry *tmp_entry = malloc(sizeof(struct database_entry));
        tmp_entry->key = tmp_key;
        tmp_entry->type = DB_STRING;

        struct json_object *json = malloc(sizeof(struct json_object));
        json->keys = malloc(sizeof(char *));
        json->keys[0] = tmp_key;
        json->vals = malloc(sizeof(struct json_value *));

        struct json_value *json_val = malloc(sizeof(struct json_value));
        json_val->child_object = NULL;
        json_val->str_val = tmp_key;
        json_val->type = JSON_STRING;

        json->vals[0] = json_val;
        json->num_keys = 1;
        json->num_vals = 1;
        json->string_size = strlen("{\"\":\"\"}") + strlen(tmp_key) * 2 + 1;

        tmp_entry->data_ptr = json_to_string(json);

        // TODO: Destroy the json object

        db_insert(mappings, tmp_entry);
    }

    // TODO: Error checking
    db_save(mappings);

    // TODO: Error checking
    db_close(mappings);

    db_initialize(&mappings, "test-save");

    for (int i = 9; i >= 0; --i) {
        char *tmp_key = (char *)malloc(sizeof(char) * 13);
        sprintf(tmp_key, "%d", i);
        tmp_key[12] = '\0';

        struct database_entry *tmp_entry = db_find(mappings, tmp_key);

        if (!tmp_entry) {
            printf("[TEST FAIL] test_db_save(): tmp_entry was null at key %s\n", tmp_key);

            return 1;
        }

        if (strcmp(tmp_entry->key, tmp_key)) {
            printf("[TEST FAIL] test_db_save(): tmp_entry->key %s does not equal expected value (%s)\n", tmp_entry->key, tmp_key);

            return 1;
        }

        if (tmp_entry->type != DB_STRING) {
            printf("[TEST FAIL] test_db_save(): tmp_entry->type %d does not equal expected value (%d)\n", tmp_entry->type, DB_STRING);

            return 1;
        }

        struct json_object *json = json_parse_string((char *)tmp_entry->data_ptr);

        if (strcmp(json->vals[0]->str_val, tmp_key)) {
            printf("[TEST FAIL] test_db_save(): tmp_entry json data value %s does not equal expected value (%s)\n", json->vals[0]->str_val, tmp_key);

            return 1;
        }

        // TODO: Destroy the json object
    }

    return 0;
}

int test_db_remove() {
    struct database_mappings *mappings;
    const char *test_name = "test-save";

    db_initialize(&mappings, "test-save");

    for (int i = 0; i < 10; i += 2) {
        char *tmp_key = (char *)malloc(sizeof(char) * 13);
        sprintf(tmp_key, "%d", i);
        tmp_key[12] = '\0';

        struct database_entry *tmp_entry = db_find(mappings, tmp_key);

        if (!tmp_entry) {
            printf("[TEST FAIL] test_db_remove(): tmp_entry was null at key %s\n", tmp_key);

            return 1;
        }

        int res = db_remove(mappings, tmp_entry);
        if (res) {
            printf("[TEST FAIL] test_db_remove(): db_remove failed with code %d key %s\n", res, tmp_key);

            return 1;
        }
    }

    for (int i = 0; i < 10; i += 2) {
        char *tmp_key = (char *)malloc(sizeof(char) * 13);
        sprintf(tmp_key, "%d", i);
        tmp_key[12] = '\0';

        struct database_entry *tmp_entry = db_find(mappings, tmp_key);

        if (tmp_entry) {
            printf("[TEST FAIL] test_db_remove(): found removed entry at key %s\n", tmp_key);

            return 1;
        }
    }

    for (int i = 1; i < 10; i += 2) {
        char *tmp_key = (char *)malloc(sizeof(char) * 13);
        sprintf(tmp_key, "%d", i);
        tmp_key[12] = '\0';

        struct database_entry *tmp_entry = db_find(mappings, tmp_key);

        if (!tmp_entry) {
            printf("[TEST FAIL] test_db_remove(): found entry that has not yet been removed at key %s\n", tmp_key);

            return 1;
        }
    }

    for (int i = 1; i < 10; i += 2) {
        char *tmp_key = (char *)malloc(sizeof(char) * 13);
        sprintf(tmp_key, "%d", i);
        tmp_key[12] = '\0';

        struct database_entry *tmp_entry = db_find(mappings, tmp_key);

        if (!tmp_entry) {
            printf("[TEST FAIL] test_db_remove(): tmp_entry was null at key %s\n", tmp_key);

            return 1;
        }

        int res = db_remove(mappings, tmp_entry);
        if (res) {
            printf("[TEST FAIL] test_db_remove(): db_remove failed with code %d key %s\n", res, tmp_key);

            return 1;
        }
    }

    for (int i = 1; i < 10; i += 2) {
        char *tmp_key = (char *)malloc(sizeof(char) * 13);
        sprintf(tmp_key, "%d", i);
        tmp_key[12] = '\0';

        struct database_entry *tmp_entry = db_find(mappings, tmp_key);

        if (tmp_entry) {
            printf("[TEST FAIL] test_db_remove(): found removed entry at key %s\n", tmp_key);

            return 1;
        }
    }

    for (int i = 0; i < 10; ++i) {
        char *tmp_key = (char *)malloc(sizeof(char) * 13);
        sprintf(tmp_key, "%d", i);
        tmp_key[12] = '\0';

        struct database_entry *tmp_entry = db_find(mappings, tmp_key);

        if (tmp_entry) {
            printf("[TEST FAIL] test_db_remove(): found removed entry at key %s\n", tmp_key);

            return 1;
        }

        int res = db_remove(mappings, tmp_entry);
        if (res == DB_REMOVE_SUCCESS) {
            printf("[TEST FAIL] test_db_remove(): db_remove failed with code %d key %s\n", res, tmp_key);

            return 1;
        }
    }

    // TODO: Error checking
    db_save(mappings);

    // TODO: Error checking
    db_close(mappings);

    char *filename = (char *)malloc(strlen("./databases/") + strlen(test_name) + 4);
    strcpy(filename, "./databases/");
    strcat(filename, test_name);
    strcat(filename, ".db\0");

    remove(filename);

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

    failed_test_counter += test_db_find();
    ++num_tests;

    failed_test_counter += test_db_insert();
    ++num_tests;

    failed_test_counter += test_db_save();
    ++num_tests;

    failed_test_counter += test_db_remove();
    ++num_tests;

    printf("[TESTING COMPLETE] DB testing %d/%d tests passed\n", num_tests - failed_test_counter, num_tests);

    return 0;
}
