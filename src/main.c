#include <stdio.h>
#include <string.h>

#include "./include/server/server.h"

#include "./tests/tests.h"

int main(int argc, char **argv) {
    if (argc > 1) {
        if (argc < 3 || argc > 4) {
            goto bad_args;
        }

        if (strcmp(argv[2], "db") == 0) {
            // TODO: Error checking and stuff
            test_db();
            return 0;
        }

        if (strcmp(argv[2], "json") == 0) {
            test_json();
            return 0;
        }

        if (strcmp(argv[2], "all")) {
            goto bad_args;
        }

        test_db();
        test_json();
    } else {
        initialize_server();
    }

    return 0;

bad_args:
    // TODO: Update me as I add more tests
    printf("Invalid argument(s)!\n");
    printf("Program format is:\n");
    printf("./main test <module>\n");
    printf("    module:\n");
    printf("        all:    run all tests\n");
    printf("        db:     run database tests\n");
    printf("        json:   run JSON parser tests\n");

    return -1;
}
