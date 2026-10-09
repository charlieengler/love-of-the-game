#ifndef NEW_JSON_HANDLER_H
#define NEW_JSON_HANDLER_H

typedef unsigned long uint64_t;

enum json_value_types { JSON_UNDEFINED, JSON_STRING, JSON_NUMBER, JSON_OBJECT, JSON_ARRAY, JSON_TRUE, JSON_FALSE, JSON_NULL };

enum json_number_types { JSON_UNDEFINED_NUMBER, JSON_INTEGER, JSON_FRACTION, JSON_EXPONENTIAL };

struct json_value {
    void *data;

    enum json_value_types type;
};

// TODO: Find a more flexible structure that can support more JSON functionality
struct json_object {
    char **keys;
    struct json_value **values;

    int num_allocated;
    int num_entries;
};

struct json_array {
    struct json_value **values;

    int length;
};

// TODO: Sanitize data before it reaches this struct to ensure it doesn't exceed the scope of long long
struct json_number {
    long long integer;
    long long fraction;
    long long exponent;

    enum json_number_types type;
};

#define JSON_WHITESPACE(input)                                                                                                                                                                         \
    case 32 /* Space */:                                                                                                                                                                               \
        ++(*input);                                                                                                                                                                                    \
        break;                                                                                                                                                                                         \
    case 10 /* Line Feed */:                                                                                                                                                                           \
        ++(*input);                                                                                                                                                                                    \
        break;                                                                                                                                                                                         \
    case 13 /* Carriage Return */:                                                                                                                                                                     \
        ++(*input);                                                                                                                                                                                    \
        break;                                                                                                                                                                                         \
    case 9 /* Horizontal Tab */:                                                                                                                                                                       \
        ++(*input);                                                                                                                                                                                    \
        break;

struct json_value *string_to_json_value(char **input);

char *json_value_to_string(struct json_value *json_val);
int destroy_json_value(struct json_value *json_val);
int destroy_json_object(struct json_object *json_obj);

struct json_value *create_string_json_value(char *str);

struct json_value *create_number_json_value(long long integer, long long fraction, long long exponent, enum json_number_types type);

struct json_value *create_object_json_value();
int json_object_add_value(struct json_object **json_obj, char *key, struct json_value *json_val);
struct json_value *json_object_get_value(struct json_object *json_obj, char *key);
int json_object_remove_value(struct json_object **json_obj, char *key);

struct json_value *create_array_json_value();
int json_array_add_value(struct json_array **json_arr, struct json_value *json_val);
struct json_value *json_array_pop_value(struct json_array *json_arr);

struct json_value *create_true_json_value();

struct json_value *create_false_json_value();

struct json_value *create_null_json_value();

struct json_value *json_object_get_value(struct json_object *json_obj, char *key);

#endif // NEW_JSON_HANDLER_H
