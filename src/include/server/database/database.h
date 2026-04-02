typedef long unsigned int uint64_t;

#define DB_MAX_NAME_LEN 128
#define DB_MAX_KEY_LEN 128
#define DB_NUM_DEFAULT_ENTRIES 256
#define DB_GROW_MULTIPLIER 2

enum entry_data_types {
    DB_STRING,
    DB_JSON,
    DB_INTEGER,
    DB_FLOAT,
    DB_UNDEFINED = -1
};

struct database_entry {
    char *key;
    enum entry_data_types type;
    void *data_ptr;
};

// TODO: Redefine all relevant data types to their cross compatible counterparts (int's to uintx_t's)
struct database_mappings {
    uint64_t num_keys;
    uint64_t num_entries;
    uint64_t num_allocated;

    char *db_name;

    char **keys;
    struct database_entry **entries;
};

uint64_t db_initialize(struct database_mappings**, char*);
struct database_entry *db_find(struct database_mappings *mappings, char *key);
int db_insert(struct database_mappings *mappings, struct database_entry *new_entry);
int db_save(struct database_mappings *mappings);
int db_close(struct database_mappings *mappings);
