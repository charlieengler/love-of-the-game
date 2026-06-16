#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/server/database/database.h"
// TODO: The contents of the database should be stored in memory, but saved to disk constantly on a separate thread
// TODO: Database calls should be asynchronous and realistically contained within a new process

int errno;

// Credit: dbj2 by Dan Bernstein
uint64_t hash(unsigned char *str) {
    unsigned long final = 5381;
    int c;

    while ((c = *str++))
        final = ((final << 5) + final) + c; /* hash * 33 + c */

    return final;
}

int db_repair(struct database_mappings *mappings) {
    // TODO: If there's an orphaned key or entry, try to restore it, or just discard it

    printf("Called repair on %s\n", mappings->db_name);

    // TODO: Return 0 on success, something else on failure
    return 0;
}

uint64_t db_initialize(struct database_mappings **mappings, char *name) {
    *mappings = (struct database_mappings *)malloc(sizeof(struct database_mappings));

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

    uint64_t num_keys = 0;
    uint64_t num_entries = 0;
    uint64_t num_allocated = DB_NUM_DEFAULT_ENTRIES;

    (*mappings)->num_keys = num_keys;
    (*mappings)->num_entries = num_entries;
    (*mappings)->num_allocated = num_allocated;

    (*mappings)->db_name = (char *)calloc(strlen(name) + 1, sizeof(char));
    strcpy((*mappings)->db_name, name);

    (*mappings)->keys = (char **)calloc(num_allocated, sizeof(char *));
    (*mappings)->entries = (struct database_entry **)calloc(num_allocated, sizeof(struct database_entry *));

    // TODO: Check the format of the db file as well
    if (fscanf(db_file, "%ld", &num_keys) >= 1) {
        // TODO: Error checking for the entries in the db file at some point
        fscanf(db_file, "%ld", &num_entries);
        fscanf(db_file, "%ld", &num_allocated);

        (*mappings)->num_allocated = num_allocated;

        (*mappings)->db_name = (char *)calloc(strlen(name) + 1, sizeof(char));
        strcpy((*mappings)->db_name, name);

        char *scanned_name = (char *)calloc(DB_MAX_NAME_LEN + 1, sizeof(char));
        // TODO: Could introduce a buffer overflow if length of name isn't properly checked elsewhere
        fscanf(db_file, "%s", scanned_name);

        if (strcmp(name, scanned_name)) {
            printf("db initialize error: names don't match\n");
            fclose(db_file);
            free(scanned_name);
            free(db_filename);
            return 0;
        }

        free(scanned_name);

        (*mappings)->keys = (char **)calloc(num_allocated, sizeof(char *));
        (*mappings)->entries = (struct database_entry **)calloc(num_allocated, sizeof(struct database_entry *));

        for (uint64_t i = 0; i < num_keys; i++) {
            struct database_entry *new_entry = (struct database_entry *)malloc(sizeof(struct database_entry));
            new_entry->key = (char *)calloc(DB_MAX_KEY_LEN + 1, sizeof(char));
            new_entry->type = DB_UNDEFINED;
            new_entry->data_ptr = NULL;

            // TODO: Could introduce a buffer overflow if length of key isn't properly checked elsewhere
            if (fscanf(db_file, "%s", new_entry->key) != 1) {
                // TODO: Better error checking that also verifies data
                printf("db initialize error: null key when loading from file\n");
                fclose(db_file);
                return 0;
            }

            // TODO: Break this into a helper function
            fscanf(db_file, "%d", &(new_entry->type));

            // TODO: This is terrible, need to find a better way to parse the data entry in the file
            while (fgetc(db_file) != '\n')
                continue;

            switch (new_entry->type) {
            case DB_STRING:
                char c;
                // TODO: Could introduce a buffer overflow if the number of chars is greater than
                //       max value of uint32_t
                uint32_t num_chars = 128;
                uint32_t i = 0;
                new_entry->data_ptr = (void *)calloc(num_chars + 1, sizeof(char));
                while ((c = fgetc(db_file)) != EOF && c != '\n') {
                    if (i == num_chars) {
                        num_chars += 128;

                        char *tmp = calloc(num_chars + 1, sizeof(char));
                        strcpy(tmp, (char *)new_entry->data_ptr);

                        new_entry->data_ptr = (void *)tmp;
                    }

                    ((char *)new_entry->data_ptr)[i] = c;

                    i++;
                }

                if (i == 0) {
                    // TODO: Better error checking that also verifies data
                    printf("db initialize error: null key when loading from file\n");
                    fclose(db_file);
                    free(db_filename);
                    return 0;
                }

                break;

            case DB_JSON:
                // TODO: Implement me
                break;

            case DB_INTEGER:
                // TODO: Implement me
                break;

            case DB_FLOAT:
                // TODO: Implement me
                break;

            default:
                printf("db intialize error: undefined entry type: %d\n", new_entry->type);
                fclose(db_file);
                free(db_filename);
                return 0;
            }

            // TODO: Error checking
            db_insert(*mappings, new_entry);
        }
    }

    db_save(*mappings);

    fclose(db_file);
    free(db_filename);

    return (*mappings)->num_entries;
}

// TODO: This function should be run on a separate thread
int db_save(struct database_mappings *mappings) {
    if (mappings->num_keys != mappings->num_entries) {
        // TODO: Error checking on the following function
        db_repair(mappings);
    }

    char *db_filename = (char *)calloc(strlen("./databases/") + strlen(mappings->db_name) + strlen(".db") + 1, sizeof(char));
    strcpy(db_filename, "./databases/");
    strcat(db_filename, mappings->db_name);
    strcat(db_filename, ".db");

    FILE *db_file = fopen(db_filename, "w");

    if (db_file == NULL) {
        printf("db save error: could not open database file: %s\n", db_filename);
        free(db_filename);
        fclose(db_file);
        return -1;
    }

    fprintf(db_file, "%ld\n", mappings->num_keys);
    fprintf(db_file, "%ld\n", mappings->num_entries);
    fprintf(db_file, "%ld\n", mappings->num_allocated);

    fprintf(db_file, "%s\n", mappings->db_name);

    for (uint64_t i = 0; i < mappings->num_allocated; ++i) {
        char *key = mappings->keys[i];
        if (key == NULL) {
            continue;
        }

        struct database_entry *entry = db_find(mappings, key);

        fprintf(db_file, "%s\n", mappings->keys[i]);

        fprintf(db_file, "%d\n", entry->type);

        switch (entry->type) {
        case DB_STRING:
            fprintf(db_file, "%s\n", (char *)(entry->data_ptr));
            break;

        case DB_JSON:
            // TODO: Implement me
            break;

        case DB_INTEGER:
            // TODO: Implement me
            break;

        case DB_FLOAT:
            // TODO: Implement me
            break;

        default:
            printf("db save error: undefined entry type\n");
            fclose(db_file);
            free(db_filename);
            return -1;
        }
    }

    fclose(db_file);
    free(db_filename);

    db_close(mappings);

    // TODO: Returns 0 on success, something else on failure
    return 0;
}

int db_close(struct database_mappings *mappings) {
    // TODO: Implement me

    printf("db_close called on %s.db\n", mappings->db_name);

    // TODO: Returns 0 on success, something else on failure
    return 0;
}

struct database_entry *db_find(struct database_mappings *mappings, char *key) {
    if (mappings->num_keys != mappings->num_entries)
        // TODO: Error checking on the following function
        db_repair(mappings);

    if (mappings->num_keys == 0) {
        printf("db find error: no keys in db\n");
        return NULL;
    }

    if (mappings->num_entries == 0) {
        printf("db find error: no entries in db\n");
        return NULL;
    }

    unsigned long index = hash((unsigned char *)key) % mappings->num_allocated;
    uint64_t num_loops = 0;
    char found_entry = 0;
    while (1) {
        ++num_loops;

        // TODO: Use a threshold value instead of the total size of mappings->num_allocated
        if (num_loops >= mappings->num_allocated) {
            // TODO: Maybe grow the database if this is encountered
            printf("db find error: could not find the given key %s\n", key);
            return NULL;
        }

        if (index >= mappings->num_allocated) {
            index = 0;
        }

        if (!mappings->keys[index]) {
            ++index;

            continue;
        }

        if (strcmp(mappings->keys[index], key) == 0) {
            found_entry = 1;

            break;
        }

        ++index;
    }

    if (!found_entry) {
        return NULL;
    }

    return mappings->entries[index];
}

uint64_t db_grow(struct database_mappings *mappings) {
    uint64_t new_size = mappings->num_allocated * DB_GROW_MULTIPLIER;
    char **new_keys = (char **)calloc(new_size, sizeof(char *));
    struct database_entry **new_entries = (struct database_entry **)calloc(new_size, sizeof(struct database_entry *));

    struct database_mappings *new_mappings = (struct database_mappings *)malloc(sizeof(struct database_mappings));

    new_mappings->num_keys = mappings->num_keys;
    new_mappings->num_entries = mappings->num_entries;
    new_mappings->num_allocated = new_size;
    new_mappings->keys = new_keys;
    new_mappings->entries = new_entries;

    for (uint64_t i = 0; i < mappings->num_keys; i++) {
        struct database_entry *old_entry = mappings->entries[i];
        if (strcmp(old_entry->key, mappings->keys[i]) != 0) {
            printf("db grow error: key and entry do not match\n");
            return 0;
        }

        db_insert(new_mappings, old_entry);
    }

    free(mappings);
    mappings = new_mappings;

    // TODO: Return the new number of possible mappings on success, 0 on failure
    return new_size;
}

int db_insert(struct database_mappings *mappings, struct database_entry *new_entry) {
    if (mappings->num_keys != mappings->num_entries) {
        // TODO: Error checking on the following function
        db_repair(mappings);
    }

    if (mappings->num_keys == mappings->num_allocated) {
        // TODO: The grow function causes issues
        uint64_t new_size = db_grow(mappings);

        if (new_size == 0) {
            printf("db grow error: new_size == 0\n");
            return -1;
        } else if (new_size <= mappings->num_allocated) {
            printf("db grow error: new_size (%ld) is the same as or less than the previous size\n", new_size);
            return -1;
        }
    }

    mappings->num_keys++;
    mappings->num_entries++;

    uint64_t index = hash((unsigned char *)new_entry->key) % mappings->num_allocated;
    uint64_t num_loops = 0;
    while (mappings->keys[index]) {
        index++;
        num_loops++;

        if (num_loops > mappings->num_allocated) {
            // TODO: Maybe grow the database if this is encountered
            printf("db insert error: database is full, but didn't grow\n");
            return -1;
        }
    }

    mappings->keys[index] = new_entry->key;
    mappings->entries[index] = new_entry;

    if (mappings->num_keys == (uint64_t)(-1) || mappings->num_entries == (uint64_t)(-1)) {
        printf("db insert error: database is absolutely full somehow\n");
        return -1;
    }

    // TODO: Return 0 on success, something else otherwise
    return 0;
}

int db_remove(struct database_mappings *mappings, struct database_entry *old_entry) {
    if (mappings->num_keys != mappings->num_entries) {
        // TODO: Error checking on the following function
        db_repair(mappings);
    }

    if (mappings->num_keys == 0) {
        printf("db remove error: no keys in db\n");
        return DB_REMOVE_NO_KEYS;
    }

    if (mappings->num_entries == 0) {
        printf("db remove error: no entries in db\n");
        return DB_REMOVE_NO_ENTRIES;
    }

    switch (old_entry->type) {
    case DB_STRING:
        free((old_entry->data_ptr));
        break;

    case DB_JSON:
        // TODO: Implement me
        break;

    case DB_INTEGER:
        // TODO: Implement me
        break;

    case DB_FLOAT:
        // TODO: Implement me
        break;

    case DB_UNDEFINED:
    default:
        printf("db remove error: undefined entry type\n");
        // TODO: Maybe handle this better
        free(old_entry->data_ptr);
    }

    unsigned long index = hash((unsigned char *)old_entry->key) % mappings->num_allocated;
    uint64_t num_loops = 0;
    while (1) {
        ++num_loops;

        // TODO: Use a threshold value instead of the total size of mappings->num_allocated
        if (num_loops >= mappings->num_allocated) {
            printf("db remove error: could not find the given key %s\n", old_entry->key);
            return DB_REMOVE_NOT_FOUND;
        }

        if (index >= mappings->num_allocated) {
            index = 0;
        }

        if (!mappings->keys[index]) {
            ++index;

            continue;
        }

        if (strcmp(mappings->keys[index], old_entry->key) == 0) {
            free(mappings->keys[index]);
            mappings->keys[index] = NULL;
            free(mappings->entries[index]);
            mappings->entries[index] = NULL;
            mappings->num_keys--;
            mappings->num_entries--;

            break;
        }

        ++index;
    }

    return DB_REMOVE_SUCCESS;
}
