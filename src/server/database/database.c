#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/server/database/database.h"
// TODO: The contents of the database should be stored in memory, but saved to disk constantly on a separate thread
// TODO: Database calls should be asynchronous and realistically contained within a new process

// Credit: dbj2 by Dan Bernstein
uint64_t hash(unsigned char *str)
{
    unsigned long final = 5381;
    int c;

    while (c = *str++)
        final = ((final << 5) + final) + c; /* hash * 33 + c */

    return final;
}

int db_repair(struct database_mappings *mappings) {
    // TODO: If there's an orphaned key or entry, try to restore it, or just discard it

    // TODO: Return 0 on success, something else on failure
    return 0;
}

uint64_t db_initialize(struct database_mappings **mappings) {
    *mappings = (struct database_mappings*)malloc(sizeof(struct database_mappings));

    (*mappings)->num_keys = 0;
    (*mappings)->num_entries = 0;
    (*mappings)->num_allocated = DB_NUM_DEFAULT_ENTRIES;
    (*mappings)->keys = (char**)calloc(DB_NUM_DEFAULT_ENTRIES, sizeof(char*));
    (*mappings)->entries = (struct database_entry**)calloc(DB_NUM_DEFAULT_ENTRIES, sizeof(struct database_entry*));

    // TODO: Returns the number of default entries on success, something else on failure
    return DB_NUM_DEFAULT_ENTRIES;
}

int db_free_mappings(struct database_mappings *mappings) {
    // TODO: Implement me

    // TODO: Returns 0 on success, something else on failure
    return 0;
}

struct database_entry *db_find(struct database_mappings *mappings, char *key) {
    unsigned long index = hash(key) % mappings->num_keys;
    uint64_t num_loops = 0;
    while(strcmp(mappings->keys[index], key) != 0) {
        index++;
        num_loops++;

        // TODO: Use a threshold value instead of the total size of mappings->num_allocated
        if(num_loops > mappings->num_allocated) {
            // TODO: Maybe grow the database if this is encountered
            printf("db find error: could not find key\n");
            return NULL;
        }
    }

    return mappings->entries[index];
}

uint64_t db_grow(struct database_mappings *mappings) {
    uint64_t new_size = mappings->num_allocated * DB_GROW_MULTIPLIER;
    char **new_keys = (char**)calloc(new_size, sizeof(char*));
    struct database_entry **new_entries = (struct database_entry**)calloc(new_size, sizeof(struct database_entry*));

    struct database_mappings *new_mappings = (struct database_mappings*)malloc(sizeof(struct database_mappings));

    new_mappings->num_keys = mappings->num_keys;
    new_mappings->num_entries = mappings->num_entries;
    new_mappings->num_allocated = new_size;
    new_mappings->keys = new_keys;
    new_mappings->entries = new_entries;

    // TODO: Implement me
    for(uint64_t i = 0; i < mappings->num_keys; i++) {
        struct database_entry *old_entry = mappings->entries[i];
        if(strcmp(old_entry->key, mappings->keys[i]) != 0) {
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
    if(mappings->num_keys != mappings->num_entries)
        // TODO: Error checking on the following function
        db_repair(mappings);

    if(mappings->num_keys == mappings->num_allocated) {
        uint64_t new_size = db_grow(mappings);
        
        if(new_size == 0) {
            printf("db grow error: new_size == 0\n");
            return -1;
        } else if(new_size <= mappings->num_allocated) {
            printf("db grow error: new_size (%ld) is the same as or less than the previous size\n", new_size);
            return -1;
        }
    }

    mappings->num_keys++;
    mappings->num_entries++;

    uint64_t index = hash(new_entry->key) % mappings->num_keys;
    uint64_t num_loops = 0;
    while(mappings->keys[index]) {
        index++;
        num_loops++;

        if(num_loops > mappings->num_allocated) {
            // TODO: Maybe grow the database if this is encountered
            printf("db insert error: database is full, but didn't grow\n");
            return -1;
        }
    }

    mappings->keys[index] = new_entry->key;
    mappings->entries[index] = new_entry;

    if(mappings->num_keys == (uint64_t)(-1) || mappings->num_entries == (uint64_t)(-1)) {
        printf("db insert error: database is absolutely full somehow\n");
        return -1;
    }

    // TODO: Return 0 on success, something else otherwise
    return 0;
}