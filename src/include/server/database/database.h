typedef long unsigned int uint64_t;

#define DB_NUM_DEFAULT_ENTRIES 256
#define DB_GROW_MULTIPLIER 2

struct database_entry {
    char *key;
    char *type;
    void *data_ptr;
};

// TODO: Redefine all relevant data types to their cross compatible counterparts (int's to uintx_t's)
struct database_mappings {
    uint64_t num_keys;
    uint64_t num_entries;
    uint64_t num_allocated;

    char **keys;
    struct database_entry **entries;
};

uint64_t db_initialize(struct database_mappings**);
struct database_entry *db_find(struct database_mappings *mappings, char *key);
int db_insert(struct database_mappings *mappings, struct database_entry *new_entry);