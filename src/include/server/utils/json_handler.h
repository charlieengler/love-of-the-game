typedef unsigned char uint8_t;

// TODO: Find a more flexible structure that can support more JSON functionality
struct json_object {
    char **keys;
    char **vals;

    int num_keys;
    int num_vals;

    int string_size;
};

struct json_object *json_initialize_object();
int json_add_entry(struct json_object*, char*, char*);
char *json_find_entry(struct json_object*, char*);
struct json_object *json_parse_string(char*);
char *json_to_string(struct json_object*, uint8_t);