#ifndef JSON_INTERNAL_H
#define JSON_INTERNAL_H

#define JSON_OBJECT_DEFAULT_ALLOC 10
#define JSON_OBJECT_GROW_MULTIPLIER 2

struct json_object;
struct json_array;
struct json_number;

char *json_string_to_string(char *json_str);
char *json_number_to_string(struct json_number *json_num);
char *json_object_to_string(struct json_object *json_obj, int depth);
char *json_array_to_string(struct json_array *json_arr, int depth);
char *json_true_to_string();
char *json_false_to_string();
char *json_null_to_string();

char *json_value_to_string_helper(struct json_value *json_val, int depth);

struct json_number *create_json_number();
struct json_object *create_json_object();
struct json_array *create_json_array();

#endif // JSON_INTERNAL_H
