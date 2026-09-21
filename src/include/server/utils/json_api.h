#ifndef JSON_API_H
#define JSON_API_H

struct json_value;

char *json_value_to_string(struct json_value *);
int destroy_json_value(struct json_value *);

#endif // JSON_API_H
