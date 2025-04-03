#include <stdio.h>
#include <string.h>

#include "./include/server/server.h"

#include "./tests/db_test.h"

int main(int argc, char **argv) {
    if(argc > 2) {
        if(argc < 3 || argc > 4) {
            // TODO: Update me as I add more tests
            printf("Invalid number of arguments!\n");
            printf("Program format is:\n");
            printf("./main test <module>\n");
            printf("    module:\n");
            printf("        all:    run all tests\n");
            printf("        db:     run database tests\n");
        }

        if(strcmp(argv[2], "db") == 0) {
            // TODO: Error checking and stuff
            test_db();
            return 0;
        }

        if(strcmp(argv[2], "all")) {
            printf("testing error: invalid argument\n");
            return -1;
        }

        test_db();
    } else {
        initialize_server();
    }

    return 0;
}