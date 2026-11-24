#include <stdio.h>
#include <string.h>

#include "./internal.h"

#include "../../../include/logging.h"

#include "../../../include/server/games/blackjack/blackjack.h"

#include "../../../include/server/database/database.h"
#include "../../../include/server/utils/json.h"

int blackjack_progress_hand(struct json_value *blackjack_db_json, char *req, int user_action, char **send_buffer) {
    check_json_type(blackjack_db_json->type, JSON_OBJECT, return 500);

    struct json_object *blackjack_db_object = (struct json_object *)blackjack_db_json->data;

    char *req_start = req;
    struct json_value *req_json = string_to_json_value(&req);
    req = req_start;

    if (!req) {
        printd("unable to convert req %s to json value\n", req);

        return 500;
    }

    check_json_type(req_json->type, JSON_OBJECT, return 500);

    struct json_object *req_object = (struct json_object *)(req_json->data);

    char *table_id;
    if (!json_object_get_value(req_object, "tableID", (void **)&table_id, JSON_STRING)) {
        printd("unable to get tableID from req_object\n");

        return 500;
    }

    struct json_object *table_object;
    struct json_value *table_json = json_object_get_value(blackjack_db_object, table_id, (void **)&table_object, JSON_OBJECT);
    if (!table_json) {
        // TODO: Potentially need to create the table here, although it should already exist
        printd("could not find table %s in blackjack DB\n", table_id);

        return 500;
    }

    struct json_number *hand_progress_num;
    struct json_value *hand_progress_json = json_object_get_value(table_object, "hand-progress", (void **)&hand_progress_num, JSON_NUMBER);
    if (!hand_progress_json) {
        hand_progress_json = create_number_json_value(NULL, -1, 0, 0, JSON_INTEGER);
        if (!hand_progress_json) {
            printd("unable to create hand_progress_json\n");

            return 500;
        }

        if (json_object_add_value(&table_object, "hand-progress", hand_progress_json, (void **)&hand_progress_num, JSON_NUMBER)) {
            printd("unable to add hand_progress_json to table_object\n");

            return 500;
        }
    }

    int hand_progress = (int)(hand_progress_num->integer);

    struct json_array *users_array;
    if (!json_object_get_value(table_object, "users", (void **)&users_array, JSON_ARRAY)) {
        printd("unable to get users array from table_object\n");

        return 500;
    }

    int num_users = users_array->length;

    struct json_object *dealer_object;
    struct json_value *dealer_json = json_object_get_value(table_object, "dealer", (void **)&dealer_object, JSON_OBJECT);
    if (!dealer_json) {
        printd("unable to get dealer object from table_object\n");

        return 500;
    }

    struct json_array *dealer_hand_arr;
    struct json_value *dealer_hand_json = json_object_get_value(dealer_object, "hand", (void **)&dealer_hand_arr, JSON_ARRAY);
    if (!dealer_hand_json) {
        dealer_hand_json = create_array_json_value(NULL);
        if (!dealer_hand_json) {
            printd("unable to create dealer_hand_json\n");

            return 500;
        }

        if (json_object_add_value(&dealer_object, "hand", dealer_hand_json, (void **)&dealer_hand_arr, JSON_ARRAY)) {
            printd("unable to add dealer_hand_json to dealer_object\n");

            return 500;
        }
    }

    char *res = req;
    if (hand_progress == -1) {
        if (json_object_remove_value(&table_object, "new-hand")) {
            printd("unable to remove new-hand from table_object\n");

            return 500;
        }

        for (int i = 0; i < num_users; ++i) {
            struct json_value *tmp_user_json = users_array->values[i];
            check_json_type(tmp_user_json->type, JSON_OBJECT, return 500);

            struct json_object *tmp_user_object = (struct json_object *)(tmp_user_json->data);

            struct json_value *tmp_user_hand_json = json_object_get_value(tmp_user_object, "hand", NULL, JSON_ARRAY);
            if (!tmp_user_hand_json) {
                tmp_user_hand_json = create_array_json_value(NULL);
                if (!tmp_user_hand_json) {
                    printd("unable to create tmp_user_hand_json\n");

                    return 500;
                }

                if (json_object_add_value(&tmp_user_object, "hand", tmp_user_hand_json, NULL, JSON_ARRAY)) {
                    printd("unable to add tmp_user_hand_json to tmp_user_object\n");

                    return 500;
                }
            }

            check_json_type(tmp_user_hand_json->type, JSON_ARRAY, return 500);

            if (deal_card(table_object, tmp_user_hand_json) == -1) {
                printd("unable to deal first user card\n");

                return 500;
            }
        }

        if (deal_card(table_object, dealer_hand_json) == -1) {
            printd("unable to deal first dealer card\n");

            return 500;
        }

        for (int i = 0; i < num_users; ++i) {
            struct json_value *tmp_user_json = users_array->values[i];

            check_json_type(tmp_user_json->type, JSON_OBJECT, return 500);

            struct json_object *tmp_user_object = (struct json_object *)(tmp_user_json->data);

            struct json_value *tmp_user_hand_json = json_object_get_value(tmp_user_object, "hand", NULL, JSON_ARRAY);
            if (!tmp_user_hand_json) {
                printd("unable to get hand from tmp_user_object\n");

                return 500;
            }

            if (deal_card(table_object, tmp_user_hand_json) == -1) {
                printd("unable to deal second user card\n");

                return 500;
            }
        }

        if (deal_card(table_object, dealer_hand_json) == -1) {
            printd("unable to deal second dealer card\n");

            return 500;
        }

        res = generate_table_res(table_json);
        if (!res) {
            printd("res was null when starting hand\n");

            return 500;
        }
    } else if (hand_progress >= 0 && hand_progress < num_users) {
        // Turn for each player

        char *req_user_id;
        struct json_value *req_user_id_json = json_object_get_value(req_object, "userID", (void **)&req_user_id, JSON_STRING);
        if (!req_user_id_json) {
            printd("unable to get userID from req_object\n");

            return 500;
        }

        struct json_value *user_json = users_array->values[hand_progress];
        if (!user_json) {
            printd("user was null at index %d\n", hand_progress);

            return 500;
        }

        check_json_type(user_json->type, JSON_OBJECT, return 500);

        struct json_object *user_object = (struct json_object *)(user_json->data);

        char *user_id;
        if (!json_object_get_value(user_object, "id", (void **)&user_id, JSON_STRING)) {
            printd("unable to get id from user_object\n");

            return 500;
        }

        if (strcmp(req_user_id, user_id)) {
            // TODO: Return error as res, not a printf
            printd("user played out of turn\n");

            // TODO: Error checking
            destroy_json_value(req_user_id_json);

            goto out;
        }

        struct json_value *user_hand_json = json_object_get_value(user_object, "hand", NULL, JSON_ARRAY);
        if (!user_hand_json) {
            printd("unable to get hand from user_object\n");

            return 500;
        }

        int progress_hand = 1;
        if (user_action == USER_HIT) {
            if (deal_card(table_object, user_hand_json) == -1) {
                printd("unable to deal card to user during turn %d\n", hand_progress);

                return 500;
            }

            if (tally_hand(user_hand_json) <= 21) {
                progress_hand = 0;
            }
        } else if (user_action == USER_STAND) {
            // TODO: Stand behavior (if any)
        } else {
            printd("invalid user action during turn %d\n", hand_progress);
            progress_hand = 0;
        }

        res = generate_table_res(table_json);
        if (!res) {
            printd("unable to generate res during turn %d\n", hand_progress);

            return 500;
        }

        if (!progress_hand) {
            goto out;
        }
    } else if (hand_progress == num_users) {
        // Dealer's turn
        struct json_object *dealer_object;
        if (!json_object_get_value(table_object, "dealer", (void **)&dealer_object, JSON_OBJECT)) {
            printd("unable to add dealer_object to table_object\n");

            return 500;
        }

        struct json_value *dealer_hand_json = json_object_get_value(dealer_object, "hand", NULL, JSON_ARRAY);
        if (!dealer_hand_json) {
            printd("unable to add hand to dealer_object\n");

            return 500;
        }

        int dealer_sum = tally_hand(dealer_hand_json);

        while (dealer_sum < 17) {
            if (deal_card(table_object, dealer_hand_json) == -1) {
                printd("unable to deal card to dealer during dealer turn\n");

                return 500;
            }

            dealer_sum = tally_hand(dealer_hand_json);
        }

        res = generate_table_res(table_json);
        if (!res) {
            printd("unable to generate res during dealer turn\n");

            return 500;
        }
    } else {
        // Hand is over

        int dealer_value = tally_hand(dealer_hand_json);
        if (dealer_value > 21) {
            dealer_value = 0;
        }

        for (int i = 0; i < num_users; ++i) {
            struct json_value *tmp_user_json = users_array->values[i];
            if (!tmp_user_json) {
                printd("tmp_user_json was null at index %d when hand was over\n", i);

                continue;
            }

            check_json_type(tmp_user_json->type, JSON_OBJECT, continue);

            struct json_object *tmp_user_object = (struct json_object *)tmp_user_json->data;

            struct json_value *tmp_user_hand_json = json_object_get_value(tmp_user_object, "hand", NULL, JSON_ARRAY);
            if (!tmp_user_hand_json) {
                printd("unable to get hand from tmp_user_object when hand was over\n");

                continue;
            }

            int user_value = tally_hand(tmp_user_hand_json);
            if (user_value > 21) {
                user_value = 0;
            }

            char *tmp_user_id;
            if (!json_object_get_value(tmp_user_object, "id", (void **)&tmp_user_id, JSON_STRING)) {
                printd("unable to get id from tmp_user_object\n");

                continue;
            }

            struct json_number *tmp_user_bet_num;
            if (!json_object_get_value(tmp_user_object, "bet", (void **)&tmp_user_bet_num, JSON_NUMBER)) {
                printd("unable to get bet from tmp_user_object\n");

                continue;
            }

            int tmp_user_bet = (int)(tmp_user_bet_num->integer);

            if (user_value > dealer_value) {
                if (update_user_balance(tmp_user_id, tmp_user_bet * 2) == -1) {
                    printd("unable to update user balance for %s (user won)\n", tmp_user_id);

                    continue;
                }
            } else if (user_value == dealer_value) {
                if (user_value) {
                    if (update_user_balance(tmp_user_id, tmp_user_bet) == -1) {
                        printd("unable to update user balance for %s (user pushed)\n", tmp_user_id);

                        continue;
                    }
                }
            }
        }

        if (json_object_remove_value(&table_object, "hand-progress") == -1) {
            printd("unable to remove hand-progress from table_object\n");
        }

        if (json_object_remove_value(&table_object, "deck") == -1) {
            printd("unable to remove deck from table_object\n");
        }

        if (json_object_remove_value(&dealer_object, "hand") == -1) {
            printd("unable to remove hand from table_object\n");
        }

        for (int i = 0; i < num_users; ++i) {
            struct json_value *tmp_user_json = users_array->values[i];
            if (!tmp_user_json) {
                printd("user was null at index %d when ending hand\n", i);

                continue;
            }

            check_json_type(tmp_user_json->type, JSON_OBJECT, continue);

            if (json_object_remove_value((struct json_object **)(&tmp_user_json->data), "hand") == -1) {
                printd("unable to remove hand from user %d\n", i);
            }

            if (json_object_remove_value((struct json_object **)(&tmp_user_json->data), "bet") == -1) {
                printd("unable to remove bet from user %d\n", i);
            }
        }

        if (json_object_add_value(&table_object, "new-hand", create_true_json_value(), NULL, JSON_TRUE)) {
            printd("unable to add new-hand to table_object\n");

            return 500;
        }

        res = generate_table_res(table_json);
        if (!res) {
            printd("unable to generate res when ending hand\n");

            return 500;
        }

        goto out;
    }

    ++hand_progress;

    ((struct json_number *)(hand_progress_json->data))->integer = hand_progress;

out:
    // TODO: Destroy all JSON objects created at the top of this function

    if (send_buffer) {
        *send_buffer = res;
    }

    return 200;
}

int blackjack_status_update(struct json_value *blackjack_db_json, char *req, char **send_buffer) {
    check_json_type(blackjack_db_json->type, JSON_OBJECT, return 500);

    struct json_object *blackjack_db_object = blackjack_db_json->data;

    char *req_start = req;
    struct json_value *req_json = string_to_json_value(&req);
    req = req_start;

    if (!req_json) {
        printd("unable to convert req %s to json\n", req);

        return 500;
    }

    check_json_type(req_json->type, JSON_OBJECT, return 500);

    struct json_object *req_object = (struct json_object *)req_json->data;

    char *table_id;
    if (!json_object_get_value(req_object, "tableID", (void **)&table_id, JSON_STRING)) {
        printd("unable to get tableID from req_object\n");

        return 500;
    }

    struct json_object *table_object;
    struct json_value *table_json = json_object_get_value(blackjack_db_object, table_id, (void **)&table_object, JSON_OBJECT);

    if (!table_json) {
        // TODO: Potentially need to create the table here, although it should already exist
        printd("unable to get table %s from blackjack DB\n", table_id);

        return 500;
    }

    struct json_number *hand_progress_num = NULL;
    if (!json_object_get_value(table_object, "hand-progress", (void **)&hand_progress_num, JSON_NUMBER)) {
        printd("unable to get hand-progress from table_object\n");
    }

    struct json_array *users_arr = NULL;
    if (!json_object_get_value(table_object, "users", (void **)&users_arr, JSON_ARRAY)) {
        printd("unable to get users array from table_obejct\n");

        return 500;
    }

    if (users_arr) {
        int num_users = users_arr->length;

        if (hand_progress_num) {
            int hand_progress = hand_progress_num->integer;

            if (hand_progress == num_users) {
                if (blackjack_progress_hand(blackjack_db_json, req, USER_NO_ACTION, send_buffer) != 200) {
                    printd("unable to progress hand\n");

                    return 500;
                }
            }
        }

        for (int i = 0; i < num_users; ++i) {
            struct json_value *current_user_json = users_arr->values[i];

            check_json_type(current_user_json->type, JSON_OBJECT, continue);

            struct json_object *current_user_object = (struct json_object *)current_user_json->data;

            char *user_id;
            if (!json_object_get_value(current_user_object, "id", (void **)&user_id, JSON_STRING)) {
                printd("failed to find user id in user object\n");

                return 500;
            }

            if (!user_id) {
                printd("user_id was null\n");

                continue;
            }

            struct json_value *users_db = db_initialize("users");

            if (!users_db) {
                printd("failed to initialize users_db\n");

                return 500;
            }

            check_json_type(users_db->type, JSON_OBJECT, continue);

            struct json_object *users_object = (struct json_object *)users_db->data;

            struct json_object *existing_user_object;

            if (!json_object_get_value(users_object, user_id, (void **)&existing_user_object, JSON_OBJECT)) {
                printd("failed to find existing user at id %s\n", user_id);

                return 500;
            }

            struct json_number *user_balance;
            struct json_value *user_balance_json = json_object_get_value(existing_user_object, "balance", (void **)&user_balance, JSON_NUMBER);
            if (!user_balance_json) {
                printd("failed to find user balance at user with id %s\n", user_id);

                return 500;
            }

            struct json_number *current_user_balance;
            if (!json_object_get_value(current_user_object, "balance", (void **)&current_user_balance, JSON_NUMBER)) {
                struct json_value *current_user_balance_json = create_number_json_value(&current_user_balance, 0, 0, 0, JSON_INTEGER);
                if (!current_user_balance_json) {
                    printd("failed to create user balance number json\n");

                    return 500;
                }

                if (json_object_add_value(&current_user_object, "balance", current_user_balance_json, NULL, JSON_NUMBER)) {
                    printd("failed to add current_user_balance_json to current_user_object\n");

                    return 500;
                }
            }

            current_user_balance->integer = user_balance->integer;
        }
    }

    if (send_buffer) {
        *send_buffer = generate_table_res(table_json);

        if (!*send_buffer) {
            printd("unable to generate res\n");

            return 500;
        }
    }

    return 200;
}
