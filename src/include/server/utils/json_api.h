#ifndef JSON_API_H
#define JSON_API_H

struct json_value;
struct json_object;

char *json_value_to_string(struct json_value *json_val);
int destroy_json_value(struct json_value *json_val);
struct json_value *json_object_get_value(struct json_object *json_obj, char *key);

#endif // JSON_API_H
