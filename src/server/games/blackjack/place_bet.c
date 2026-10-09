#include <stdio.h>
#include <string.h>

#include "internal.h"

#include "../../../include/server/games/blackjack/blackjack.h"

#include "../../../include/server/utils/json.h"

char *blackjack_place_bet(struct json_value *db_json, char *req) {
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
    json_object_get_value((struct json_object *)(req_json->data), "userID", (void **)&user_id, JSON_STRING);

    struct json_number *bet_num;
    // TODO: Error checking
    json_object_get_value((struct json_object *)(req_json->data), "bet", (void **)&bet_num, JSON_NUMBER);

    int bet = (int)(bet_num->integer);

    struct json_value *table_json = json_object_get_value((struct json_object *)(db_json->data), table_id, NULL, JSON_OBJECT);

    if (!table_json) {
        // TODO: Error checking
        blackjack_join_table(db_json, req);
    }

    struct json_object *table_object = (struct json_object *)(table_json->data);

    struct json_array *users_array;
    // TODO: Error checking
    json_object_get_value(table_object, "users", (void **)&users_array, JSON_ARRAY);

    struct json_value *found_user_json = NULL;
    for (int i = 0; i < users_array->length; ++i) {
        struct json_value *tmp_user_json = users_array->values[i];
        if (tmp_user_json->type != JSON_OBJECT) {
            // TODO: Fail
        }
        struct json_object *tmp_user_object = tmp_user_json->data;

        char *tmp_user_id;
        // TODO: Error checking
        json_object_get_value(tmp_user_object, "id", (void **)&tmp_user_id, JSON_STRING);

        if (!strcmp(user_id, tmp_user_id)) {
            found_user_json = tmp_user_json;
            break;
        }
    }

    if (!found_user_json) {
        // TODO: Error checking
        blackjack_join_table(db_json, req);
    }

    if (found_user_json->type != JSON_OBJECT) {
        // TODO: Fail
    }

    struct json_object *found_user_object = found_user_json->data;

    struct json_number *user_bet_num;
    // TODO: Error checking
    json_object_get_value(found_user_object, "bet", (void **)&user_bet_num, JSON_NUMBER);

    user_bet_num->integer = bet;

    // TODO: Better res
    char *res = req;
    return res;
}
