#include <stdlib.h>

#include "./internal.h"

#include "../../../include/logging.h"

#include "../../../include/server/database/database.h"
#include "../../../include/server/users/authenticate.h"
#include "../../../include/server/utils/json.h"

int validate_user(char *req) {
    struct json_value *users_db_json = db_initialize("users");

    if (!users_db_json) {
        printd("unable to initialize users DB\n");

        return -1;
    }

    char *find_user_res;
    int code = find_user(users_db_json, req, &find_user_res);
    if (code != 200) {
        printd("unable to find user, returned code %d", code);

        return -1;
    }
    char *find_user_res_start = find_user_res;

    struct json_value *db_found_user_json = string_to_json_value(&find_user_res);

    find_user_res = find_user_res_start;

    if (!db_found_user_json) {
        printd("unable to convert find_user_res %s to json value\n", find_user_res);

        return -1;
    }

    if (db_found_user_json->type == JSON_FALSE) {
        printd("user does not exist\n");

        return 1;
    }

    free(find_user_res);

    destroy_json_value(db_found_user_json);

    return 0;
}
