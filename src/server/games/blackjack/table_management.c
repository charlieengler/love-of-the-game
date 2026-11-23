#include <stdio.h>
#include <string.h>

#include "./internal.h"

#include "../../../include/logging.h"

#include "../../../include/server/games/blackjack/blackjack.h"

#include "../../../include/server/database/database.h"
#include "../../../include/server/utils/json.h"

static char *generate_table_res(struct json_value *table_json) {
    if (!table_json) {
        printd("table_management.c->generate_table_res(): table_json is null\n");

        return NULL;
    }

    if (table_json->type != JSON_OBJECT) {
        printd("table_management.c->generate_table_res(): table_json is of type %d, expected %d\n", table_json->type, JSON_OBJECT);

        return NULL;
    }

    struct json_object *table_object = (struct json_object *)(table_json->data);

    struct json_number *hand_progress_num;
    struct json_value *hand_progress_json = json_object_get_value(table_object, "hand-progress", (void **)&hand_progress_num, JSON_NUMBER);
    if (!hand_progress_json) {
        printd("table_management.c->generate_table_res(): unable to get hand-progress from table_object\n");

        return NULL;
    }

    int hand_progress;
    if (!hand_progress_json) {
        hand_progress = -1;
    } else {
        hand_progress = (int)(hand_progress_num->integer);
    }

    struct json_array *users_array;
    struct json_value *users_json = json_object_get_value(table_object, "users", (void **)&users_array, JSON_ARRAY);
    if (!users_json) {
        printd("table_management.c->generate_table_res(): unable to get users array from table_object\n");

        return NULL;
    }

    int num_users = users_array->length;

    struct json_object *dealer_object;
    struct json_value *dealer_json = json_object_get_value(table_object, "dealer", (void **)&dealer_object, JSON_OBJECT);
    if (!dealer_json) {
        printd("table_management.c->generate_table_res(): unable to get dealer from table_object\n");

        return NULL;
    }

    struct json_array *dealer_hand_arr;
    struct json_value *dealer_hand_json = json_object_get_value(dealer_object, "hand", (void **)&dealer_hand_arr, JSON_ARRAY);
    if (!dealer_hand_json) {
        dealer_hand_json = create_array_json_value(NULL);

        if (!dealer_hand_json) {
            printd("table_management.c->generate_table_res(): unable to create dealer_hand_json\n");

            return NULL;
        }

        if (!json_object_add_value(&dealer_object, "hand", dealer_hand_json, (void **)&dealer_hand_arr, JSON_ARRAY)) {
            printd("table_management.c->generate_table_res(): unable to add dealer_hand_json to dealer_object\n");

            return NULL;
        }
    }

    struct json_value *new_hand_json = json_object_get_value(table_object, "new-hand", NULL, JSON_TRUE);
    if (!new_hand_json) {
        printd("table_management.c->generate_table_res(): unable to get new_hand_json from table_object\n");
    }

    struct json_value *res_dealer_hand_json;
    struct json_value *active_user_json;
    if (hand_progress >= 0 && hand_progress < num_users) {
        active_user_json = users_array->values[hand_progress];
    } else {
        active_user_json = dealer_json;

        res_dealer_hand_json = dealer_hand_json;
    }

    if (hand_progress < num_users) {
        struct json_array *res_dealer_hand_arr;

        res_dealer_hand_json = create_array_json_value(&res_dealer_hand_arr);
        if (!res_dealer_hand_json) {
            printd("table_management.c->generate_table_res(): unable to create res_dealer_hand_json\n");

            return NULL;
        }

        if (dealer_hand_arr->length > 1) {
            if (json_array_add_value(&res_dealer_hand_arr, dealer_hand_arr->values[1])) {
                printd("table_management.c->generate_table_res(): unable to add dealer card to res_dealer_hand_arr\n");

                return NULL;
            }
        }
    }

    struct json_object *res_object;
    struct json_value *res_json = create_object_json_value(&res_object);
    if (!res_json) {
        printd("table_management.c->generate_table_res(): unable to create res_json\n");

        return NULL;
    }

    if (hand_progress >= num_users) {
        struct json_value *hand_ended_json = create_true_json_value();
        if (!hand_ended_json) {
            printd("table_management.c->generate_table_res(): unable to create hand_ended_json\n");

            return NULL;
        }

        if (json_object_add_value(&res_object, "handEnded", hand_ended_json, NULL, JSON_TRUE)) {
            printd("table_management.c->generate_table_res(): unable to add true hand_ended_json to res_object\n");

            return NULL;
        }
    } else {
        struct json_value *hand_ended_json = create_false_json_value();
        if (!hand_ended_json) {
            printd("table_management.c->generate_table_res(): unable to create hand_ended_json\n");

            return NULL;
        }

        if (json_object_add_value(&res_object, "handEnded", hand_ended_json, NULL, JSON_FALSE)) {
            printd("table_management.c->generate_table_res(): unable to add false hand_ended_json to res_object\n");

            return NULL;
        }
    }

    if (new_hand_json) {
        if (json_object_add_value(&res_object, "newHand", new_hand_json, NULL, JSON_TRUE)) {
            printd("table_management.c->generate_table_res(): unable to add new_hand_json to res_object\n");

            return NULL;
        }
    }

    if (json_object_add_value(&res_object, "users", users_json, NULL, JSON_ARRAY)) {
        printd("table_management.c->generate_table_res(): unable to add users_json to res_object\n");

        return NULL;
    }

    if (json_object_add_value(&res_object, "dealerCards", res_dealer_hand_json, NULL, JSON_ARRAY)) {
        printd("table_management.c->generate_table_res(): unable to add res_dealer_hand_json to res_object\n");

        return NULL;
    }

    if (json_object_add_value(&res_object, "activeUser", active_user_json, NULL, JSON_OBJECT)) {
        printd("table_management.c->generate_table_res(): unable to add active_user_json to res_object\n");

        return NULL;
    }

    char *res = json_value_to_string(res_json);
    if (!res) {
        printd("table_management.c->generate_table_res(): unable to convert res_json to string\n");

        return NULL;
    }

    return res;
}

int blackjack_progress_hand(struct json_value *blackjack_db_json, char *req, int user_action, char **send_buffer) {
    if (blackjack_db_json->type != JSON_OBJECT) {
        printd("table_management.c->blackjack_progress_hand(): blackjack_db_json is of type %d, expected %d\n", blackjack_db_json->type, JSON_OBJECT);

        return 500;
    }

    struct json_object *blackjack_db_object = (struct json_object *)blackjack_db_json->data;

    char *req_start = req;
    struct json_value *req_json = string_to_json_value(&req);
    req = req_start;

    if (!req) {
        printd("table_management.c->blackjack_progress_hand(): unable to convert req %s to json value\n", req);

        return 500;
    }

    // TODO: Macro with callback for verifying JSON object type
    if (req_json->type != JSON_OBJECT) {
        printd("table_management.c->blackjack_progress_hand(): req_json is of type %d, expected %d\n", req_json->type, JSON_OBJECT);

        return 500;
    }

    struct json_object *req_object = (struct json_object *)(req_json->data);

    char *table_id;
    if (!json_object_get_value(req_object, "tableID", (void **)&table_id, JSON_STRING)) {
        printd("table_management.c->blackjack_progress_hand(): unable to get tableID from req_object\n");

        return 500;
    }

    struct json_object *table_object;
    struct json_value *table_json = json_object_get_value(blackjack_db_object, table_id, (void **)&table_object, JSON_OBJECT);
    if (!table_json) {
        // TODO: Potentially need to create the table here, although it should already exist
        printf("table_management.c->blackjack_progress_hand(): could not find table %s in blackjack DB\n", table_id);

        return 500;
    }

    struct json_number *hand_progress_num;
    struct json_value *hand_progress_json = json_object_get_value(table_object, "hand-progress", (void **)&hand_progress_num, JSON_NUMBER);
    if (!hand_progress_json) {
        hand_progress_json = create_number_json_value(NULL, -1, 0, 0, JSON_INTEGER);
        if (!hand_progress_json) {
            printd("table_management.c->blackjack_progress_hand(): unable to create hand_progress_json\n");

            return 500;
        }

        if (json_object_add_value(&table_object, "hand-progress", hand_progress_json, (void **)&hand_progress_num, JSON_NUMBER)) {
            printd("table_management.c->blackjack_progress_hand(): unable to add hand_progress_json to table_object\n");

            return 500;
        }
    }

    int hand_progress = (int)(hand_progress_num->integer);

    struct json_array *users_array;
    if (!json_object_get_value(table_object, "users", (void **)&users_array, JSON_ARRAY)) {
        printd("table_management.c->blackjack_progress_hand(): unable to get users array from table_object\n");

        return 500;
    }

    int num_users = users_array->length;

    struct json_object *dealer_object;
    struct json_value *dealer_json = json_object_get_value(table_object, "dealer", (void **)&dealer_object, JSON_OBJECT);
    if (!dealer_json) {
        printd("table_management.c->blackjack_progress_hand(): unable to get dealer object from table_object\n");

        return 500;
    }

    struct json_array *dealer_hand_arr;
    struct json_value *dealer_hand_json = json_object_get_value(dealer_object, "hand", (void **)&dealer_hand_arr, JSON_ARRAY);
    if (!dealer_hand_json) {
        dealer_hand_json = create_array_json_value(NULL);
        if (!dealer_hand_json) {
            printd("table_management.c->blackjack_progress_hand(): unable to create dealer_hand_json\n");

            return 500;
        }

        if (json_object_add_value(&dealer_object, "hand", dealer_hand_json, (void **)&dealer_hand_arr, JSON_ARRAY)) {
            printd("table_management.c->blackjack_progress_hand(): unable to add dealer_hand_json to dealer_object\n");

            return 500;
        }
    }

    char *res = req;
    if (hand_progress == -1) {
        if (json_object_remove_value(&table_object, "new-hand")) {
            printd("table_management.c->blackjack_progress_hand(): unable to remove new-hand from table_object\n");

            return 500;
        }

        for (int i = 0; i < num_users; ++i) {
            struct json_value *tmp_user_json = users_array->values[i];
            if (tmp_user_json->type != JSON_OBJECT) {
                printd("table_management.c->blackjack_progress_hand(): tmp_user_json is of type %d, expected %d\n", tmp_user_json->type, JSON_OBJECT);

                return 500;
            }

            struct json_object *tmp_user_object = (struct json_object *)(tmp_user_json->data);

            struct json_value *tmp_user_hand_json = json_object_get_value(tmp_user_object, "hand", NULL, JSON_ARRAY);
            if (!tmp_user_hand_json) {
                tmp_user_hand_json = create_array_json_value(NULL);
                if (!tmp_user_hand_json) {
                    printd("table_management.c->blackjack_progress_hand(): unable to create tmp_user_hand_json\n");

                    return 500;
                }

                if (json_object_add_value(&tmp_user_object, "hand", tmp_user_hand_json, NULL, JSON_ARRAY)) {
                    printd("table_management.c->blackjack_progress_hand(): unable to add tmp_user_hand_json to tmp_user_object\n");

                    return 500;
                }
            }

            if (tmp_user_hand_json->type != JSON_ARRAY) {
                printd("table_management.c->blackjack_progress_hand(): tmp_user_hand_json is of type %d, expected %d\n", tmp_user_hand_json->type, JSON_ARRAY);

                return 500;
            }

            if (deal_card(table_object, tmp_user_hand_json) == -1) {
                printd("table_management.c->blackjack_progress_hand(): unable to deal first user card\n");

                return 500;
            }
        }

        if (deal_card(table_object, dealer_hand_json) == -1) {
            printd("table_management.c->blackjack_progress_hand(): unable to deal first dealer card\n");

            return 500;
        }

        for (int i = 0; i < num_users; ++i) {
            struct json_value *tmp_user_json = users_array->values[i];
            if (tmp_user_json->type != JSON_OBJECT) {
                printd("table_management.c->blackjack_progress_hand(): tmp_user_json is of type %d, expected %d\n", tmp_user_json->type, JSON_OBJECT);

                return 500;
            }

            struct json_object *tmp_user_object = (struct json_object *)(tmp_user_json->data);

            struct json_value *tmp_user_hand_json = json_object_get_value(tmp_user_object, "hand", NULL, JSON_ARRAY);
            if (!tmp_user_hand_json) {
                printd("table_management.c->blackjack_progress_hand(): unable to get hand from tmp_user_object\n");

                return 500;
            }

            if (deal_card(table_object, tmp_user_hand_json) == -1) {
                printd("table_management.c->blackjack_progress_hand(): unable to deal second user card\n");

                return 500;
            }
        }

        if (deal_card(table_object, dealer_hand_json) == -1) {
            printd("table_management.c->blackjack_progress_hand(): unable to deal second dealer card\n");

            return 500;
        }

        res = generate_table_res(table_json);
        if (!res) {
            printd("table_management.c->blackjack_progress_hand(): res was null when starting hand\n");

            return 500;
        }
    } else if (hand_progress >= 0 && hand_progress < num_users) {
        // Turn for each player

        char *req_user_id;
        struct json_value *req_user_id_json = json_object_get_value(req_object, "userID", (void **)&req_user_id, JSON_STRING);
        if (!req_user_id_json) {
            printd("table_management.c->blackjack_progress_hand(): unable to get userID from req_object\n");

            return 500;
        }

        struct json_value *user_json = users_array->values[hand_progress];
        if (!user_json) {
            printd("table_management.c->blackjack_progress_hand(): user was null at index %d\n", hand_progress);

            return 500;
        }

        if (user_json->type != JSON_OBJECT) {
            printd("table_management.c->blackjack_progress_hand(): user_json is of type %d, expected %d\n", user_json->type, JSON_OBJECT);

            return 500;
        }

        struct json_object *user_object = (struct json_object *)(user_json->data);

        char *user_id;
        if (!json_object_get_value(user_object, "id", (void **)&user_id, JSON_STRING)) {
            printd("table_management.c->blackjack_progress_hand(): unable to get id from user_object\n");

            return 500;
        }

        if (strcmp(req_user_id, user_id)) {
            // TODO: Return error as res, not a printf
            printd("table_management.c->blackjack_progress_hand(): user played out of turn\n");

            // TODO: Error checking
            destroy_json_value(req_user_id_json);

            goto out;
        }

        struct json_value *user_hand_json = json_object_get_value(user_object, "hand", NULL, JSON_ARRAY);
        if (!user_hand_json) {
            printd("table_management.c->blackjack_progress_hand(): unable to get hand from user_object\n");

            return 500;
        }

        int progress_hand = 1;
        if (user_action == USER_HIT) {
            if (deal_card(table_object, user_hand_json) == -1) {
                printd("table_management.c->blackjack_progress_hand(): unable to deal card to user during turn %d\n", hand_progress);

                return 500;
            }

            if (tally_hand(user_hand_json) <= 21) {
                progress_hand = 0;
            }
        } else if (user_action == USER_STAND) {
            // TODO: Stand behavior (if any)
        } else {
            printd("table_management.c->blackjack_progress_hand(): invalid user action during turn %d\n", hand_progress);
            progress_hand = 0;
        }

        res = generate_table_res(table_json);
        if (!res) {
            printd("table_management.c->blackjack_progress_hand(): unable to generate res during turn %d\n", hand_progress);

            return 500;
        }

        if (!progress_hand) {
            goto out;
        }
    } else if (hand_progress == num_users) {
        // Dealer's turn
        struct json_object *dealer_object;
        if (!json_object_get_value(table_object, "dealer", (void **)&dealer_object, JSON_OBJECT)) {
            printd("table_management.c->blackjack_progress_hand(): unable to add dealer_object to table_object\n");

            return 500;
        }

        struct json_value *dealer_hand_json = json_object_get_value(dealer_object, "hand", NULL, JSON_ARRAY);
        if (!dealer_hand_json) {
            printd("table_management.c->blackjack_progress_hand(): unable to add hand to dealer_object\n");

            return 500;
        }

        int dealer_sum = tally_hand(dealer_hand_json);

        while (dealer_sum < 17) {
            if (deal_card(table_object, dealer_hand_json) == -1) {
                printd("table_management.c->blackjack_progress_hand(): unable to deal card to dealer during dealer turn\n");

                return 500;
            }

            dealer_sum = tally_hand(dealer_hand_json);
        }

        res = generate_table_res(table_json);
        if (!res) {
            printd("table_management.c->blackjack_progress_hand(): unable to generate res during dealer turn\n");

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
                printd("table_management.c->blackjack_progress_hand(): tmp_user_json was null at index %d when hand was over\n", i);

                continue;
            }

            if (tmp_user_json->type != JSON_OBJECT) {
                printd("table_management.c->blackjack_progress_hand(): tmp_user_json is of type %d, expected %d\n", tmp_user_json->type, JSON_OBJECT);

                continue;
            }

            struct json_object *tmp_user_object = (struct json_object *)tmp_user_json->data;

            struct json_value *tmp_user_hand_json = json_object_get_value(tmp_user_object, "hand", NULL, JSON_ARRAY);
            if (!tmp_user_hand_json) {
                printd("table_management.c->blackjack_progress_hand(): unable to get hand from tmp_user_object when hand was over\n");

                continue;
            }

            int user_value = tally_hand(tmp_user_hand_json);
            if (user_value > 21) {
                user_value = 0;
            }

            char *tmp_user_id;
            if (!json_object_get_value(tmp_user_object, "id", (void **)&tmp_user_id, JSON_STRING)) {
                printd("table_management.c->blackjack_progress_hand(): unable to get id from tmp_user_object\n");

                continue;
            }

            struct json_number *tmp_user_bet_num;
            if (!json_object_get_value(tmp_user_object, "bet", (void **)&tmp_user_bet_num, JSON_NUMBER)) {
                printd("table_management.c->blackjack_progress_hand(): unable to get bet from tmp_user_object\n");

                continue;
            }

            int tmp_user_bet = (int)(tmp_user_bet_num->integer);

            if (user_value > dealer_value) {
                if (update_user_balance(tmp_user_id, tmp_user_bet * 2) == -1) {
                    printd("table_management.c->blackjack_progress_hand(): unable to update user balance for %s (user won)\n", tmp_user_id);

                    continue;
                }
            } else if (user_value == dealer_value) {
                if (user_value) {
                    if (update_user_balance(tmp_user_id, tmp_user_bet) == -1) {
                        printd("table_management.c->blackjack_progress_hand(): unable to update user balance for %s (user pushed)\n", tmp_user_id);

                        continue;
                    }
                }
            }
        }

        if (json_object_remove_value(&table_object, "hand-progress") == -1) {
            printd("table_management.c->blackjack_progress_hand(): unable to remove hand-progress from table_object\n");
        }

        if (json_object_remove_value(&table_object, "deck") == -1) {
            printd("table_management.c->blackjack_progress_hand(): unable to remove deck from table_object\n");
        }

        if (json_object_remove_value(&dealer_object, "hand") == -1) {
            printd("table_management.c->blackjack_progress_hand(): unable to remove hand from table_object\n");
        }

        for (int i = 0; i < num_users; ++i) {
            struct json_value *tmp_user_json = users_array->values[i];
            if (!tmp_user_json) {
                printd("table_management.c->blackjack_progress_hand() user was null at index %d when ending hand\n", i);

                continue;
            }

            if (tmp_user_json->type != JSON_OBJECT) {
                printd("table_management.c->blackjack_progress_hand(): tmp_user_json is of type %d, expected %d when ending hand\n", tmp_user_json->type, JSON_OBJECT);

                continue;
            }

            if (json_object_remove_value((struct json_object **)(&tmp_user_json->data), "hand") == -1) {
                printd("table_management.c->blackjack_progress_hand(): unable to remove hand from user %d\n", i);
            }

            if (json_object_remove_value((struct json_object **)(&tmp_user_json->data), "bet") == -1) {
                printd("table_management.c->blackjack_progress_hand(): unable to remove bet from user %d\n", i);
            }
        }

        if (json_object_add_value(&table_object, "new-hand", create_true_json_value(), NULL, JSON_TRUE)) {
            printd("table_management.c->blackjack_progress_hand(): unable to add new-hand to table_object\n");

            return 500;
        }

        res = generate_table_res(table_json);
        if (!res) {
            printd("table_management.c->blackjack_progress_hand(): unable to generate res when ending hand\n");

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
    if (blackjack_db_json->type != JSON_OBJECT) {
        printd("table_management.c->blackjack_status_update(): blackjack_db_json is of type %d, expected %d when ending hand\n", blackjack_db_json->type, JSON_OBJECT);

        return 500;
    }

    struct json_object *blackjack_db_object = blackjack_db_json->data;

    char *req_start = req;
    struct json_value *req_json = string_to_json_value(&req);
    req = req_start;

    if (!req_json) {
        printd("table_management.c->blackjack_status_update(): unable to convert req %s to json\n", req);

        return 500;
    }

    // TODO: Macro with callback for verifying JSON object type
    if (req_json->type != JSON_OBJECT) {
        printd("table_management.c->blackjack_status_update(): req_json is of type %d, expected %d when ending hand\n", req_json->type, JSON_OBJECT);

        return 500;
    }

    struct json_object *req_object = (struct json_object *)req_json->data;

    char *table_id;
    if (!json_object_get_value(req_object, "tableID", (void **)&table_id, JSON_STRING)) {
        printd("table_management.c->blackjack_status_update(): unable to get tableID from req_object\n");

        return 500;
    }

    struct json_object *table_object;
    struct json_value *table_json = json_object_get_value(blackjack_db_object, table_id, (void **)&table_object, JSON_OBJECT);

    if (!table_json) {
        // TODO: Potentially need to create the table here, although it should already exist
        printd("table_management.c->blackjack_status_update(): unable to get table %s from blackjack DB\n", table_id);

        return 500;
    }

    struct json_number *hand_progress_num = NULL;
    if (!json_object_get_value(table_object, "hand-progress", (void **)&hand_progress_num, JSON_NUMBER)) {
        printd("table_management.c->blackjack_status_update(): unable to get hand-progress from table_object\n");

        return 500;
    }

    struct json_array *users_arr = NULL;
    if (!json_object_get_value(table_object, "users", (void **)&users_arr, JSON_ARRAY)) {
        printd("table_management.c->blackjack_status_update(): unable to get users array from table_obejct\n");

        return 500;
    }

    if (hand_progress_num && users_arr) {
        int hand_progress = hand_progress_num->integer;
        int num_users = users_arr->length;

        if (hand_progress == num_users) {
            if (blackjack_progress_hand(blackjack_db_json, req, USER_NO_ACTION, send_buffer) != 200) {
                printd("table_management.c->blackjack_status_update(): unable to progress hand\n");

                return 500;
            }
        }
    }

    if (send_buffer) {
        *send_buffer = generate_table_res(table_json);

        if (!*send_buffer) {
            printd("table_management.c->blackjack_status_update(): unable to generate res\n");

            return 500;
        }
    }

    return 200;
}
