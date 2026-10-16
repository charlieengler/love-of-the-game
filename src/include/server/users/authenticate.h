#ifndef AUTHENTICATE_H
#define AUTHENTICATE_H

struct json_value;

char *login_user(struct json_value *db_json, char *req);
char *find_user(struct json_value *db_json, char *req);
char *create_user(struct json_value *db_json, char *req);
char *logout_user(struct json_value *db_json, char *req);

#endif // AUTHENTICATE_H
