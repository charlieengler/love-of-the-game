#include <string.h>

#include "internal.h"

#include "../../../include/logging.h"

#include "../../../include/server/database/database.h"
#include "../../../include/server/users/authenticate.h"
#include "../../../include/server/utils/json.h"

int blackjack_join_table(struct json_value *db_json, char *req, char **send_buffer) {
    if (db_json->type != JSON_OBJECT) {
        printd("join_table.c->blackjack_join_table(): db_json->type is %d, expected %d\n", db_json->type, JSON_OBJECT);

        goto fail;
    }

    struct json_object *db_object = (struct json_object *)db_json->data;

    char *req_start = req;
    struct json_value *req_json = string_to_json_value(&req);
    req = req_start;

    if (!req_json) {
        printd("join_table.c->blackjack_join_table(): req_json is null\n");

        goto fail;
    }

    // TODO: Macro with callback for verifying JSON object type
    if (req_json->type != JSON_OBJECT) {
        printd("join_table.c->blackjack_join_table(): req_json->type is %d, expected %d\n", req_json->type, JSON_OBJECT);

        goto fail;
    }

    struct json_object *req_object = (struct json_object *)req_json->data;

    char *table_id;
    if (!json_object_get_value(req_object, "tableID", (void **)&table_id, JSON_STRING)) {
        printd("join_table.c->blackjack_join_table(): unable to get tableID from req_object\n");

        goto fail;
    }

    char *user_id;
    struct json_value *user_id_json = json_object_get_value(req_object, "userID", (void **)&user_id, JSON_STRING);
    if (!user_id_json) {
        printd("join_table.c->blackjack_join_table(): unable to get userID from req_object\n");

        goto fail;
    }

    if (validate_user(req)) {
        printd("join_table.c->blackjack_join_table(): failed to validate user when joining blackjack table\n");

        goto fail;
    }

    struct json_object *table_object;
    struct json_value *table_json = json_object_get_value((struct json_object *)(db_json->data), table_id, (void **)&table_object, JSON_OBJECT);

    if (!table_json) {
        table_json = create_object_json_value(&table_object);
        if (!table_json || !table_object) {
            printd("join_table.c->blackjack_join_table(): failed to create table_object\n");

            goto fail;
        }

        int ret = json_object_add_value(&db_object, table_id, table_json, (void **)&table_object, JSON_OBJECT);
        if (ret) {
            printd("join_table.c->blackjack_join_table(): failed to add table to blackjack DB, json_object_add_value() returned %d\n", ret);

            destroy_json_value(table_json);

            goto fail;
        }

        struct json_value *users_json = create_array_json_value(NULL);
        if (!users_json) {
            printd("join_table.c->blackjack_join_table(): failed to create users array\n");

            goto fail;
        }

        ret = json_object_add_value(&table_object, "users", users_json, NULL, JSON_ARRAY);
        if (ret) {
            printd("join_table.c->blackjack_join_table(): failed to add users array to table object, json_object_add_value() returned %d\n", ret);

            destroy_json_value(table_json);
            destroy_json_value(users_json);

            goto fail;
        }

        struct json_value *dealer_json = create_object_json_value(NULL);
        if (!dealer_json) {
            printd("join_table.c->blackjack_join_table(): failed to create dealer object\n");

            destroy_json_value(table_json);
            destroy_json_value(users_json);
            destroy_json_value(dealer_json);

            goto fail;
        }

        ret = json_object_add_value(&table_object, "dealer", dealer_json, NULL, JSON_OBJECT);
        if (ret) {
            printd("join_table.c->blackjack_join_table(): failed to add dealer object to table object, json_object_add_value() returned %d\n", ret);

            destroy_json_value(table_json);
            destroy_json_value(users_json);
            destroy_json_value(dealer_json);

            goto fail;
        }

        // TODO: Destroy the new true json value
        ret = json_object_add_value(&table_object, "new-hand", create_true_json_value(), NULL, JSON_TRUE);
        if (ret) {
            printd("join_table.c->blackjack_join_table(): failed to add new hand boolean to table object, json_object_add_value() returned %d\n", ret);

            destroy_json_value(table_json);
            destroy_json_value(users_json);
            destroy_json_value(dealer_json);

            goto fail;
        }
    }

    struct json_array *users_array;
    if (!json_object_get_value(table_object, "users", (void **)&users_array, JSON_ARRAY) || !users_array) {
        printd("join_table.c->blackjack_join_table(): failed to get users array from table object\n");

        goto fail;
    }

    struct json_value *found_user_json = NULL;
    for (int i = 0; i < users_array->length; ++i) {
        struct json_value *tmp_user_json = users_array->values[i];

        if (!tmp_user_json) {
            printd("join_table.c->blackjack_join_table(): user was null at index %d in users array\n", i);

            continue;
        }

        if (tmp_user_json->type != JSON_OBJECT) {
            printd("join_table.c->blackjack_join_table(): user entry type at index %d in users array is %d, expected %d\n", i, tmp_user_json->type, JSON_OBJECT);

            continue;
        }

        struct json_object *tmp_user_object = (struct json_object *)tmp_user_json->data;

        char *tmp_user_id;
        if (!json_object_get_value(tmp_user_object, "id", (void **)&tmp_user_id, JSON_STRING)) {
            printd("join_table.c->blackjack_join_table(): failed to get user id from user entry at index %d in users array\n", i);

            continue;
        }

        if (!strcmp(user_id, tmp_user_id)) {
            found_user_json = tmp_user_json;
            break;
        }
    }

    if (!found_user_json) {
        struct json_object *found_user_object;

        found_user_json = create_object_json_value(&found_user_object);
        if (!found_user_json || !found_user_object) {
            printd("join_table.c->blackjack_join_table(): unable to create user object\n");

            goto fail;
        }

        int ret = json_object_add_value(&found_user_object, "id", user_id_json, NULL, JSON_STRING);
        if (ret) {
            printd("join_table.c->blackjack_join_table(): failed to add id to user object, json_object_add_value() returned %d\n", ret);

            goto fail;
        }

        struct json_value *bet_json = create_number_json_value(NULL, 0, 0, 0, JSON_INTEGER);
        if (!bet_json) {
            printd("join_table.c->blackjack_join_table(): unable to create bet number\n");

            goto fail;
        }

        ret = json_object_add_value(&found_user_object, "bet", bet_json, NULL, JSON_NUMBER);
        if (ret) {
            printd("join_table.c->blackjack_join_table(): failed to add bet to user object, json_object_add_value() returned %d\n", ret);

            goto fail;
        }

        json_array_add_value(&users_array, found_user_json);
        if (ret) {
            printd("join_table.c->blackjack_join_table(): failed to add user object to user array, json_object_add_value() returned %d\n", ret);

            goto fail;
        }
    }

    // TODO: Better res
    *send_buffer = req;

    return 200;

fail:
    // TODO: Helpful error message in res
    return 500;
}
