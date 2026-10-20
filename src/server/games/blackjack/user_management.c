#include <stdlib.h>

#include "./internal.h"

#include "../../../include/server/database/database.h"
#include "../../../include/server/users/authenticate.h"
#include "../../../include/server/utils/json.h"

int validate_user(char *req) {
    // TODO: Error checking
    struct json_value *users_db_json = db_initialize("users");

    // TODO: Error checking
    char *find_user_res = find_user(users_db_json, req);
    char *find_user_res_start = find_user_res;

    // TODO: Error checking
    struct json_value *db_found_user_json = string_to_json_value(&find_user_res);

    if (db_found_user_json->type == JSON_FALSE) {
        return 1;
    }

    free(find_user_res_start);

    destroy_json_value(db_found_user_json);

    return 0;
}
