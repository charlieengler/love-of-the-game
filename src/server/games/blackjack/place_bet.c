#include <stdio.h>
#include <string.h>

#include "internal.h"

#include "../../../include/logging.h"

#include "../../../include/server/games/blackjack/blackjack.h"

#include "../../../include/server/database/database.h"
#include "../../../include/server/utils/json.h"

// TODO: Make get_user_by_id function and replace prelude of check_user_balance and update_user_balance
int check_user_balance(char *user_id) {
    struct json_value *users_db_json = db_initialize("users");
    if (!users_db_json) {
        printd("unable to initialize users DB\n");

        return -1;
    }

    check_json_type(users_db_json->type, JSON_OBJECT, return -1);

    struct json_object *users_db_object = (struct json_object *)users_db_json->data;

    struct json_object *user_object;
    if (!json_object_get_value(users_db_object, user_id, (void **)&user_object, JSON_OBJECT)) {
        printd("unable to get user %s from users_db_object\n", user_id);

        return -1;
    }

    struct json_number *user_balance;
    if (!json_object_get_value(user_object, "balance", (void **)&user_balance, JSON_NUMBER)) {
        printd("unable to get balance from user_object for user %s\n", user_id);

        return -1;
    }

    return user_balance->integer;
}

int update_user_balance(char *user_id, int amount) {
    struct json_value *users_db_json = db_initialize("users");
    if (!users_db_json) {
        printd("unable to initialize users DB\n");

        return -1;
    }

    check_json_type(users_db_json->type, JSON_OBJECT, return -1);

    struct json_object *users_db_object = (struct json_object *)users_db_json->data;

    struct json_object *user_object;
    if (!json_object_get_value(users_db_object, user_id, (void **)&user_object, JSON_OBJECT)) {
        printd("unable to get user %s from users_db_object\n", user_id);

        return -1;
    }

    struct json_number *user_balance;
    if (!json_object_get_value(user_object, "balance", (void **)&user_balance, JSON_NUMBER)) {
        printd("unable to get balance from user %s\n", user_id);

        return -1;
    }

    user_balance->integer += amount;

    db_save(users_db_json, "users");

    return 0;
}

int blackjack_place_bet(struct json_value *blackjack_db_json, char *req, char **send_buffer) {
    check_json_type(blackjack_db_json->type, JSON_OBJECT, return 500);

    struct json_object *db_object = (struct json_object *)blackjack_db_json->data;

    char *req_start = req;
    struct json_value *req_json = string_to_json_value(&req);
    req = req_start;

    if (!req_json) {
        printd("unable to convert req %s string to json value\n", req);

        return 500;
    }

    check_json_type(req_json->type, JSON_OBJECT, return 500);

    struct json_object *req_object = (struct json_object *)req_json->data;

    char *table_id;
    if (!json_object_get_value(req_object, "tableID", (void **)&table_id, JSON_STRING)) {
        printd("unable to get tableID from req_object\n");

        return 500;
    }

    char *user_id;
    if (!json_object_get_value(req_object, "userID", (void **)&user_id, JSON_STRING)) {
        printd("unable to get userID from req_object\n");

        return 500;
    }

    if (validate_user(req)) {
        printf("failed to validate user\n");

        return 500;
    }

    struct json_object *table_object;
    struct json_value *table_json = json_object_get_value(db_object, table_id, (void **)&table_object, JSON_OBJECT);

    if (!table_json) {
        int ret = blackjack_join_table(blackjack_db_json, req, send_buffer);
        if (ret != 200) {
            printd("unable to join blackjack table, blackjack_join_table() returned %d\n", ret);

            return ret;
        }
    }

    struct json_array *users_array;
    if (!json_object_get_value(table_object, "users", (void **)&users_array, JSON_ARRAY)) {
        printd("unable to get users array from table_object\n");

        return 500;
    }

    int already_searched = 0;

find_user:
    struct json_value *found_user_json = NULL;
    for (int i = 0; i < users_array->length; ++i) {
        struct json_value *tmp_user_json = users_array->values[i];
        if (!tmp_user_json) {
            printd("tmp_user_json was null at index %d\n", i);

            continue;
        }

        check_json_type(tmp_user_json->type, JSON_OBJECT, continue);

        struct json_object *tmp_user_object = tmp_user_json->data;

        char *tmp_user_id;
        if (!json_object_get_value(tmp_user_object, "id", (void **)&tmp_user_id, JSON_STRING)) {
            printd("unable to get user id from tmp_user_object at index %d\n", i);

            continue;
        }

        if (!strcmp(user_id, tmp_user_id)) {
            found_user_json = tmp_user_json;
            break;
        }
    }

    if (!found_user_json) {
        int ret = blackjack_join_table(blackjack_db_json, req, send_buffer);
        if (ret != 200) {
            printd("found_user_json was null and unable to join blackjack table, blackjack_join_table() returned %d\n", ret);

            return 500;
        }

        if (already_searched) {
            printd("already attempted to find and add user to table\n");

            return 500;
        }

        already_searched = 1;

        goto find_user;
    }

    check_json_type(found_user_json->type, JSON_OBJECT, return 500);

    struct json_object *found_user_object = found_user_json->data;

    struct json_number *bet_num;
    if (!json_object_get_value(req_object, "bet", (void **)&bet_num, JSON_NUMBER)) {
        printd("unable to find bet value in req_object\n");

        return 500;
    }

    int bet = (int)(bet_num->integer);

    struct json_number *user_bet_num;
    struct json_value *user_bet_json = json_object_get_value(found_user_object, "bet", (void **)&user_bet_num, JSON_NUMBER);
    if (!user_bet_json) {
        user_bet_json = create_number_json_value(&user_bet_num, 0, 0, 0, JSON_INTEGER);
        if (!user_bet_json) {
            printd("unable to create user_bet_json\n");

            return 500;
        }

        if (!json_object_add_value(&found_user_object, "bet", user_bet_json, NULL, JSON_NUMBER)) {
            printd("unable to add user_bet_json to found_user_object\n");

            return 500;
        }
    }

    check_json_type(user_bet_json->type, JSON_NUMBER, return 500);

    if (check_user_balance(user_id) < bet) {
        bet = 0;
    }

    if (update_user_balance(user_id, -bet)) {
        printd("failed to update user balance for user %s\n", user_id);

        return 500;
    }

    user_bet_num->integer = bet;

    // TODO: Better res
    *send_buffer = req;

    return 200;
}
