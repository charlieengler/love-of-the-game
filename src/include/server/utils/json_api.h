#ifndef JSON_API_H
#define JSON_API_H

#include "json_parser.h"

struct json_value;
struct json_object;
struct json_array;

char *json_value_to_string(struct json_value *json_val);
int destroy_json_value(struct json_value *json_val);

struct json_value *create_string_json_value(char *str);

struct json_value *create_number_json_value(long long integer, long long fraction, long long exponent, enum json_number_types type);

struct json_value *create_object_json_value();
int json_object_add_value(struct json_object **json_obj, char *key, struct json_value *json_val);

struct json_value *create_array_json_value();
int json_array_add_value(struct json_array **json_arr, struct json_value *json_val);

struct json_value *create_true_json_value();

struct json_value *create_false_json_value();

struct json_value *create_null_json_value();

struct json_value *json_object_get_value(struct json_object *json_obj, char *key);

#endif // JSON_API_H
