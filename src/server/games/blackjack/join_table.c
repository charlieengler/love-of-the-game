#include <stdio.h>
#include <string.h>

#include "internal.h"

#include "../../../include/server/utils/json.h"

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

    char *table_id;
    // TODO: Error checking
    json_object_get_value((struct json_object *)(req_json->data), "tableID", (void **)&table_id, JSON_STRING);

    char *user_id;
    // TODO: Error checking
    struct json_value *user_id_json = json_object_get_value((struct json_object *)(req_json->data), "userID", (void **)&user_id, JSON_STRING);

    struct json_object *table_object;
    struct json_value *table_json = json_object_get_value((struct json_object *)(db_json->data), table_id, (void **)&table_object, JSON_OBJECT);

    if (!table_json) {
        table_json = create_object_json_value(&table_object);

        json_object_add_value((struct json_object **)(&db_json->data), table_id, table_json, (void **)&table_object, JSON_OBJECT);

        struct json_value *users_json = create_array_json_value(NULL);

        json_object_add_value(&table_object, "users", users_json, NULL, JSON_ARRAY);

        struct json_value *dealer_json = create_object_json_value(NULL);

        json_object_add_value(&table_object, "dealer", dealer_json, NULL, JSON_OBJECT);
    }

    struct json_array *users_array;
    // TODO: Error checking
    json_object_get_value(table_object, "users", (void **)&users_array, JSON_ARRAY);

    struct json_value *found_user_json = NULL;
    for (int i = 0; i < users_array->length; ++i) {
        struct json_value *tmp_user_json = users_array->values[i];
        if (tmp_user_json->type != JSON_OBJECT) {
            // TODO: Fail
        }

        char *tmp_user_id;
        // TODO: Error checking
        json_object_get_value((struct json_object *)(tmp_user_json->data), "id", (void **)&tmp_user_id, JSON_STRING);

        if (!strcmp(user_id, tmp_user_id)) {
            found_user_json = tmp_user_json;
            break;
        }
    }

    if (!found_user_json) {
        struct json_object *found_user_object;

        // TODO: Error checking
        found_user_json = create_object_json_value(&found_user_object);

        // TODO: Error checking
        json_object_add_value(&found_user_object, "id", user_id_json, NULL, JSON_STRING);

        struct json_value *bet_json = create_number_json_value(NULL, 0, 0, 0, JSON_INTEGER);

        // TODO: Error checking
        json_object_add_value(&found_user_object, "bet", bet_json, NULL, JSON_NUMBER);

        // TODO: Error checking
        json_array_add_value(&users_array, found_user_json);
    }

    printf("Joined table %s as %s\n", table_id, user_id);

    // TODO: Better res
    char *res = req;
    return res;
}
