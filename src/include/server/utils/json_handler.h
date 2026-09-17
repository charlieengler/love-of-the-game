typedef unsigned long uint64_t;

enum json_data_types {
    JSON_UNDEFINED,
    JSON_NESTED,
    // This should be a string that is already formatted in the JSON format
    JSON_FORMATTED_STRING,
    JSON_STRING,
    JSON_INTEGER,
    JSON_FLOAT,
    // JSON_ARRAY,
};

struct json_value;

// TODO: Find a more flexible structure that can support more JSON functionality
struct json_object {
    char **keys;
    struct json_value **vals;

    uint64_t num_keys;
    uint64_t num_vals;

    uint64_t string_size;
};

struct json_value {
    struct json_object *child_object;

    char *str_val;
    uint64_t int_val;
    double float_val;

    // void *array_val;
    // uint64_t array_size;

    enum json_data_types type;
};

struct json_object *json_initialize_object();
int json_add_entry(struct json_object *, char *, struct json_value *);
struct json_value *json_add_string_entry(struct json_object *json_obj, char *key, char *init_val);
struct json_value *json_find_entry(struct json_object *, char *);
int json_remove_entry(struct json_object *json_obj, char *key);
struct json_object *json_parse_string(char *);
char *json_to_string(struct json_object *);
int json_destroy_object(struct json_object *json);
int json_destroy_value(struct json_value *json_val);
