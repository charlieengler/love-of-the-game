#ifndef AUTHENTICATE_H
#define AUTHENTICATE_H

struct json_value;

int login_user(struct json_value *db_json, char *req, char **send_buffer);
int find_user(struct json_value *db_json, char *req, char **send_buffer);
int create_user(struct json_value *db_json, char *req, char **send_buffer);
int logout_user(struct json_value *db_json, char *req, char **send_buffer);

#endif // AUTHENTICATE_H
