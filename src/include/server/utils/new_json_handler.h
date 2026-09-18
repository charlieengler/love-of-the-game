#ifndef NEW_JSON_HANDLER_H
#define NEW_JSON_HANDLER_H

typedef unsigned long uint64_t;

enum json_value_types {
    JSON_UNDEFINED,
    JSON_STRING,
    // This should be a string that is already formatted in the JSON format
    JSON_NUMBER,
    JSON_OBJECT,
    JSON_ARRAY,
    JSON_TRUE,
    JSON_FALSE,
    JSON_NULL
};

enum json_number_types { JSON_UNDEFINED_NUMBER, JSON_INTEGER, JSON_FRACTION, JSON_EXPONENTIAL };

struct json_value {
    void *data;

    enum json_value_types type;
};

// TODO: Find a more flexible structure that can support more JSON functionality
struct json_object {
    char **keys;
    struct json_value **values;

    uint64_t num_allocated;
    uint64_t num_entries;
};

struct json_array {
    struct json_value **values;

    uint64_t length;
};

// TODO: Sanitize data before it reaches this struct to ensure it doesn't exceed the scope of long long
struct json_number {
    long long integer;
    long double fraction;
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

#endif // NEW_JSON_HANDLER_H
