#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/server/database/database.h"
#include "../../include/server/utils/json_api.h"
// TODO: The contents of the database should be stored in memory, but saved to disk constantly on a separate thread
// TODO: Database calls should be asynchronous and realistically contained within a new process

struct json_value *db_initialize(char *name) {
    if (strlen(name) <= 0) {
        printf("db initialize error: db name length <= 0\n");
        return 0;
    }

    char *db_filename = (char *)calloc(strlen("./databases/") + strlen(name) + strlen(".db") + 1, sizeof(char));
    strcpy(db_filename, "./databases/");
    strcat(db_filename, name);
    strcat(db_filename, ".db");

    FILE *db_file = fopen(db_filename, "a+");

    if (db_file == NULL) {
        printf("db initialize error: could not open database file: %s\n", db_filename);
        free(db_filename);
        fclose(db_file);
        return 0;
    }

    char *db_buffer = NULL;
    long length;
    // TODO: Error checking
    fseek(db_file, 0, SEEK_END);
    // TODO: Error checking
    length = ftell(db_file);
    // TODO: Error checking
    fseek(db_file, 0, SEEK_SET);
    db_buffer = calloc(length, sizeof(char));
    char *db_buffer_start = db_buffer;
    if (db_buffer) {
        fread(db_buffer, 1, length, db_file);
    }
    fclose(db_file);

    struct json_value *json_val = create_object_json_value();
    if (length > 1) {
        json_val = string_to_json_value(&db_buffer);
    }
    db_buffer = db_buffer_start;
    free(db_buffer);

    if (json_val->type != JSON_OBJECT) {
        // TODO: Error message with reason for failure
        goto fail;
    }

    return json_val;

fail:
    free(json_val);
    return NULL;
}

// TODO: This function should be run on a separate thread
int db_save(struct json_value *db, char *db_name) {
    char *db_filename = (char *)calloc(strlen("./databases/") + strlen(db_name) + strlen(".db") + 1, sizeof(char));
    strcpy(db_filename, "./databases/");
    strcat(db_filename, db_name);
    strcat(db_filename, ".db");

    FILE *db_file = fopen(db_filename, "w");

    if (db_file == NULL) {
        printf("db save error: could not open database file: %s\n", db_filename);
        free(db_filename);
        fclose(db_file);
        return -1;
    }

    char *db_data_str = json_value_to_string(db);

    if (db_data_str) {
        fputs(db_data_str, db_file);

        free(db_data_str);
    }

    fclose(db_file);
    free(db_filename);

    // TODO: Returns 0 on success, something else on failure
    return 0;

fail:
    return -1;
}
