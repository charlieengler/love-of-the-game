struct json_value;

struct json_value *db_initialize(char *name);
int db_save(struct json_value *db, char *db_name);
