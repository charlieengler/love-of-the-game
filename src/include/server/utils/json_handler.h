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
char *json_to_string(struct json_object*);