#include <stdio.h>
#include <string.h>

#include "db_test.h"

// TODO: Create a standardized test function suite

int main(int argc, char **argv) {
    if(argc < 2 || argc > 3) {
        // TODO: Update me as I add more tests
        printf("Invalid number of arguments!\n");
        printf("Program format is:\n");
        printf("./test <module>\n");
        printf("    module:\n");
        printf("        all:    run all tests\n");
        printf("        db:     run database tests\n");
    }

    if(strcmp(argv[1], "db") == 0) {
        // TODO: Error checking and stuff
        test_database();
        return 0;
    }

    if(strcmp(argv[1], "all")) {
        printf("testing error: invalid argument\n");
        return -1;
    }

    test_database();

    return 0;
}