#include <stdio.h>
#include <string.h>

#include "internal.h"

#include "../../../include/server/games/blackjack/blackjack.h"

#include "../../../include/server/database/database.h"
#include "../../../include/server/utils/json.h"

int check_user_balance(char *user_id) {
    // TODO: Error checking
    struct json_value *users_db_json = db_initialize("users");

    struct json_object *users_db_object = (struct json_object *)users_db_json->data;

    struct json_object *user_object;
    // TODO: Error checking
    json_object_get_value(users_db_object, user_id, (void **)&user_object, JSON_OBJECT);

    struct json_number *user_balance;
    // TODO: Error checking
    json_object_get_value(user_object, "balance", (void **)&user_balance, JSON_NUMBER);

    return user_balance->integer;
}

int update_user_balance(char *user_id, int amount) {
    // TODO: Error checking
    struct json_value *users_db_json = db_initialize("users");

    struct json_object *users_db_object = (struct json_object *)users_db_json->data;

    struct json_object *user_object;
    // TODO: Error checking
    json_object_get_value(users_db_object, user_id, (void **)&user_object, JSON_OBJECT);

    struct json_number *user_balance;
    // TODO: Error checking
    json_object_get_value(user_object, "balance", (void **)&user_balance, JSON_NUMBER);

    user_balance->integer += amount;

    db_save(users_db_json, "users");

    return 0;
}

int blackjack_place_bet(struct json_value *db_json, char *req, char **send_buffer) {
    if (db_json->type != JSON_OBJECT) {
        // TODO: Fail
    }

    struct json_object *db_object = (struct json_object *)db_json->data;

    char *req_start = req;
    // TODO: Error checking
    struct json_value *req_json = string_to_json_value(&req);
    req = req_start;

    // TODO: Macro with callback for verifying JSON object type
    if (req_json->type != JSON_OBJECT) {
        // TODO: Fail
    }

    struct json_object *req_object = (struct json_object *)req_json->data;

    char *table_id;
    // TODO: Error checking
    json_object_get_value(req_object, "tableID", (void **)&table_id, JSON_STRING);

    char *user_id;
    // TODO: Error checking
    json_object_get_value(req_object, "userID", (void **)&user_id, JSON_STRING);

    if (validate_user(req)) {
        printf("Failed to validate user when placing bet\n");

        // TODO: Fail
    }

    struct json_object *table_object;
    struct json_value *table_json = json_object_get_value(db_object, table_id, (void **)&table_object, JSON_OBJECT);

    if (!table_json) {
        // TODO: Error checking
        blackjack_join_table(db_json, req, send_buffer);
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
        blackjack_join_table(db_json, req, send_buffer);
    }

    // TODO: If had to join the table, make sure found_user_json set

    if (found_user_json->type != JSON_OBJECT) {
        // TODO: Fail
    }

    struct json_object *found_user_object = found_user_json->data;

    struct json_number *bet_num;
    // TODO: Error checking
    json_object_get_value(req_object, "bet", (void **)&bet_num, JSON_NUMBER);

    int bet = (int)(bet_num->integer);

    struct json_number *user_bet_num;
    // TODO: Error checking
    struct json_value *user_bet_json = json_object_get_value(found_user_object, "bet", (void **)&user_bet_num, JSON_NUMBER);
    if (!user_bet_json) {
        // TODO: Error checking
        user_bet_json = create_number_json_value(&user_bet_num, 0, 0, 0, JSON_INTEGER);

        // TODO: Error checking
        json_object_add_value(&found_user_object, "bet", user_bet_json, NULL, JSON_NUMBER);
    }

    if (check_user_balance(user_id) < bet) {
        bet = 0;
    }

    if (update_user_balance(user_id, -bet)) {
        printf("Failed to update user balance\n");

        // TODO: Fail
    }

    user_bet_num->integer = bet;

    // TODO: Better res
    *send_buffer = req;

    return 200;
}
