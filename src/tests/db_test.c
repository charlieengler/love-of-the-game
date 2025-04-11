#include <stdio.h>
#include <string.h>

#include "test_tools.h"
#include "db_test.h"

#include "../src/include/server/database/database.h"

// TODO: Need to test with and without an existing db file
int test_db_initialize_no_file() {
    printf("[*] Beginning Tests (test_db_initialize_no_file)\n");

    struct database_mappings *mappings;
    const char *test_name = "test-initialize";
    int num_passed = 0;

    // TODO: Error checking
    if(db_initialize(&mappings, "test-initialize") != DB_NUM_DEFAULT_ENTRIES) {
        printf("[-] Test Failed (test_db_initialize_no_file): Database failed to initialize\n");
        return num_passed;
    } else {
        printf("[+] Test Passed (test_db_initialize_no_file): Database initialization returned correct value\n");
        num_passed++;
    }

    if(mappings == NULL) {
        printf("[-] Test Failed (test_db_initialize_no_file): Database mappings was null\n");
        return num_passed;
    } else {
        printf("[+] Test Passed (test_db_initialize_no_file): Database mappings was not null\n");
        num_passed++;
    }

    if(mappings->num_keys != 0) {
        printf("[-] Test Failed (test_db_initialize_no_file): Number of keys was not zero (%ld)\n", mappings->num_keys);
    } else {
        printf("[+] Test Passed (test_db_initialize_no_file): Number of keys was zero\n");
        num_passed++;
    }

    if(mappings->num_entries != 0) {
        printf("[-] Test Failed (test_db_initialize_no_file): Number of entries was not zero (%ld)\n", mappings->num_entries);
    } else {
        printf("[+] Test Passed (test_db_initialize_no_file): Number of entries was zero\n");
        num_passed++;
    }

    if(mappings->num_allocated != DB_NUM_DEFAULT_ENTRIES) {
        printf("[-] Test Failed (test_db_initialize_no_file): Number of allocated slots was not the default value (%ld)\n", mappings->num_allocated);
    } else {
        printf("[+] Test Passed (test_db_initialize_no_file): Number of allocated slots was the default value\n");
        num_passed++;
    }

    if(strcmp(mappings->db_name, test_name)) {
        printf("[-] Test Failed (test_db_initialize_no_file): Database names did not match (%s, %s)\n", mappings->db_name, test_name);
    } else {
        printf("[+] Test Passed (test_db_initialize_no_file): Database names matched\n");
        num_passed++;
    }

    if(mappings->keys == NULL) {
        printf("[-] Test Failed (test_db_initialize_no_file): Database **keys was null\n");
    } else {
        printf("[+] Test Passed (test_db_initialize_no_file): Database **keys was not null\n");
        num_passed++;
    }

    if(mappings->entries == NULL) {
        printf("[-] Test Failed (test_db_initialize_no_file): Database **entries was null\n");
    } else {
        printf("[+] Test Passed (test_db_initialize_no_file): Database **entries was not null\n");
        num_passed++;
    }

    db_close(mappings);

    return num_passed;
}

int test_db() {
    // TODO: Set up database and run tests on it
    printf("Testing database\n");

    // TODO: Error checking
    test_db_initialize_no_file();

    // TODO: Return 0 on success and something else on failure
    return 0;
}