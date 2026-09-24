#include <stdio.h>
#include <string.h>

#include "internal.h"

#include "../../../include/server/utils/json_api.h"

char *blackjack_join_table(struct json_value *db_json, char *req) {
    if (db_json->type != JSON_OBJECT) {
        // TODO: Fail
    }

    char *req_start = req;
    // TODO: Error checking
    struct json_value *req_json = string_to_json_value(&req);
    req = req_start;

    // TODO: Macro with callback for verifying JSON object type
    if (req_json->type != JSON_OBJECT) {
        // TODO: Fail
    }

    // TODO: Error checking
    struct json_value *table_id_json = json_object_get_value((struct json_object *)(req_json->data), "tableID");
    if (table_id_json->type != JSON_STRING) {
        // TODO: Fail
    }
    char *table_id = (char *)(table_id_json->data);

    // TODO: Error checking
    struct json_value *user_id_json = json_object_get_value((struct json_object *)(req_json->data), "userID");
    if (user_id_json->type != JSON_STRING) {
        // TODO: Fail
    }
    char *user_id = (char *)(user_id_json->data);

    struct json_value *table_json = json_object_get_value((struct json_object *)(db_json->data), table_id);
    struct json_object *table_object;

    if (!table_json) {
        table_json = create_object_json_value();
        if (table_json->type != JSON_OBJECT) {
            // TODO: Fail
        }

        table_object = (struct json_object *)(table_json->data);

        json_object_add_value((struct json_object **)(&db_json->data), table_id, table_json);

        struct json_value *users_json = create_array_json_value();

        json_object_add_value(&table_object, "users", users_json);

        struct json_value *dealer_json = create_object_json_value();

        json_object_add_value(&table_object, "dealer", dealer_json);
    }

    if (table_json->type != JSON_OBJECT) {
        // TODO: Fail
    }

    table_object = (struct json_object *)(table_json->data);

    // TODO: Error checking
    struct json_value *users_json = json_object_get_value(table_object, "users");
    if (users_json->type != JSON_ARRAY) {
        // TODO: Error
    }

    struct json_array *users_array = (struct json_array *)(users_json->data);

    struct json_value *found_user_json = NULL;
    for (int i = 0; i < users_array->length; ++i) {
        struct json_value *tmp_user_json = users_array->values[i];
        if (tmp_user_json->type != JSON_OBJECT) {
            // TODO: Fail
        }

        // TODO: Error checking
        struct json_value *tmp_user_id_json = json_object_get_value((struct json_object *)(tmp_user_json->data), "id");
        if (tmp_user_id_json->type != JSON_STRING) {
            // TODO: Fail
        }

        char *tmp_user_id = (char *)tmp_user_id_json->data;

        if (!strcmp(user_id, tmp_user_id)) {
            found_user_json = tmp_user_json;
            break;
        }
    }

    if (!found_user_json) {
        found_user_json = create_object_json_value();
        if (found_user_json->type != JSON_OBJECT) {
            // TODO: Fail
        }

        // TODO: Error checking
        json_object_add_value((struct json_object **)(&(found_user_json->data)), "id", user_id_json);

        struct json_value *bet_json = create_number_json_value(0, 0, 0, JSON_INTEGER);
        if (bet_json->type != JSON_NUMBER) {
            // TODO: Fail
        }

        // TODO: Error checking
        json_object_add_value((struct json_object **)(&(found_user_json->data)), "bet", bet_json);

        // TODO: Error checking
        json_array_add_value(&users_array, found_user_json);
    }

    printf("Joined table %s as %s\n", table_id, user_id);

    // TODO: Better res
    char *res = req;
    return res;
}
