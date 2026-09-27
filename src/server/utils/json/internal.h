#ifndef JSON_INTERNAL_H
#define JSON_INTERNAL_H

char *json_string_to_string(char *json_str, int depth);
char *json_number_to_string(struct json_number *json_num, int depth);
char *json_object_to_string(struct json_object *json_obj, int depth);
char *json_array_to_string(struct json_array *json_arr, int depth);
char *json_true_to_string(int depth);
char *json_false_to_string(int depth);
char *json_null_to_string(int depth);

char *json_value_to_string_helper(struct json_value *json_val, int depth);

#endif // JSON_INTERNAL_H
