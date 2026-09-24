#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./internal.h"

#include "../../../include/server/games/blackjack/blackjack.h"

#include "../../../include/server/database/database.h"
#include "../../../include/server/utils/json_api.h"

char *generate_card_count_str(int num_users, int *user_hand_counts) {
    char *card_count_str = (char *)malloc((num_users * 3 + 1) * sizeof(char));
    char card_count[3] = {0};
    sprintf(card_count, "%d", user_hand_counts[0]);
    strcpy(card_count_str, card_count);

    for (int i = 1; i < num_users; ++i) {
        sprintf(card_count, ",%d", user_hand_counts[i]);
        strcat(card_count_str, card_count);
    }

    card_count_str[num_users * 3] = '\0';

    return card_count_str;
}

// TODO: Proper bet handling and payouts
char *blackjack_progress_hand(struct json_value *db_json, char *req, int user_action) {
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

    struct json_object *req_object = (struct json_object *)(req_json->data);

    // TODO: Error checking
    struct json_value *table_id_json = json_object_get_value((struct json_object *)(req_json->data), "tableID");
    if (table_id_json->type != JSON_STRING) {
        // TODO: Fail
    }
    char *table_id = (char *)(table_id_json->data);

    struct json_value *table_json = json_object_get_value((struct json_object *)(db_json->data), table_id);

    if (table_json == NULL) {
        // TODO: Potentially need to create the table here, although it should already exist
        printf("blackjack error: could not find entry %s in database when starting hand\n", table_id);

        return NULL;
    }

    if (table_json->type != JSON_OBJECT) {
        // TODO: Fail
    }

    struct json_object *table_object = (struct json_object *)(table_json->data);

    int hand_progress;
    // TODO: Error checking
    struct json_value *hand_progress_json = json_object_get_value(table_object, "hand-progress");

    if (!hand_progress_json) {
        hand_progress_json = create_number_json_value(-1, 0, 0, JSON_INTEGER);

        // TODO: Error checking
        json_object_add_value(&table_object, "hand-progress", hand_progress_json);
    }

    if (hand_progress_json->type != JSON_NUMBER) {
        // TODO: Fail
    }

    hand_progress = (int)(((struct json_number *)(hand_progress_json->data))->integer);

    // TODO: Error checking
    struct json_value *users_json = json_object_get_value(table_object, "users");
    if (users_json->type != JSON_ARRAY) {
        // TODO: Fail
    }

    struct json_array *users_array = (struct json_array *)(users_json->data);
    int num_users = users_array->length;

    // TODO: Error checking
    struct json_value *dealer_json = json_object_get_value(table_object, "dealer");
    if (dealer_json->type != JSON_OBJECT) {
        // TODO: Fail
    }

    struct json_value *dealer_hand_json = json_object_get_value((struct json_object *)(dealer_json->data), "hand");
    if (!dealer_hand_json) {
        dealer_hand_json = create_array_json_value();

        json_object_add_value((struct json_object **)(&dealer_json->data), "hand", dealer_hand_json);
    }

    if (dealer_hand_json->type != JSON_ARRAY) {
        // TODO: Fail
    }

    char *res = req;
    if (hand_progress == -1) {
        for (int i = 0; i < num_users; ++i) {
            struct json_value *tmp_user_json = users_array->values[i];
            if (tmp_user_json->type != JSON_OBJECT) {
                // TODO: Fail
            }

            struct json_object *tmp_user_object = (struct json_object *)(tmp_user_json->data);

            struct json_value *tmp_user_hand_json = json_object_get_value(tmp_user_object, "hand");
            if (!tmp_user_hand_json) {
                tmp_user_hand_json = create_array_json_value();

                if (tmp_user_hand_json->type != JSON_ARRAY) {
                    // TODO: Fail
                }

                json_object_add_value(&tmp_user_object, "hand", tmp_user_hand_json);
            }

            if (tmp_user_hand_json->type != JSON_ARRAY) {
                // TODO: Fail
            }

            // TODO: Error checking
            deal_card(table_object, tmp_user_hand_json);
        }

        // TODO: Error checking
        deal_card(table_object, dealer_hand_json);

        for (int i = 0; i < num_users; ++i) {
            struct json_value *tmp_user_json = users_array->values[i];
            if (tmp_user_json->type != JSON_OBJECT) {
                // TODO: Fail
            }

            struct json_object *tmp_user_object = (struct json_object *)(tmp_user_json->data);

            struct json_value *tmp_user_hand_json = json_object_get_value(tmp_user_object, "hand");

            if (tmp_user_hand_json->type != JSON_ARRAY) {
                // TODO: Fail
            }

            // TODO: Error checking
            deal_card(table_object, tmp_user_hand_json);
        }

        // TODO: Error checking
        deal_card(table_object, dealer_hand_json);

        // TODO: Helper for constructing res
        struct json_value *res_json = create_object_json_value();
        if (res_json->type != JSON_OBJECT) {
            // TODO: Fail
        }

        struct json_object *return_object = (struct json_object *)(res_json->data);

        // TODO: Error checking
        json_object_add_value(&return_object, "users", users_json);

        // TODO: Error checking
        struct json_value *res_dealer_hand_json = create_array_json_value();
        if (res_dealer_hand_json->type != JSON_ARRAY) {
            // TODO: Fail
        }

        json_array_add_value((struct json_array **)(&res_dealer_hand_json->data), ((struct json_array *)(dealer_hand_json->data))->values[1]);

        // TODO: Error checking
        json_object_add_value(&return_object, "dealerCards", res_dealer_hand_json);

        // TODO: Error checking
        res = json_value_to_string(res_json);
    } else if (hand_progress >= 0 && hand_progress < num_users) {
        // Turn for each player

        // TODO: Error checking
        struct json_value *req_user_id_json = json_object_get_value(req_object, "userID");
        if (req_user_id_json->type != JSON_STRING) {
            // TODO: Fail
        }

        char *req_user_id = (char *)(req_user_id_json->data);

        struct json_value *user_json = users_array->values[hand_progress];
        if (user_json->type != JSON_OBJECT) {
            // TODO: Fail
        }

        struct json_object *user_object = (struct json_object *)(user_json->data);

        struct json_value *user_id_json = json_object_get_value(user_object, "id");
        if (user_id_json->type != JSON_STRING) {
            // TODO: Fail
        }

        char *user_id = (char *)(user_id_json->data);

        if (strcmp(req_user_id, user_id)) {
            // TODO: Return error as res, not a printf
            printf("User ID did not match!\n");

            // TODO: Error checking
            destroy_json_value(req_user_id_json);

            goto out;
        }

        // TODO: Error checking
        struct json_value *user_hand_json = json_object_get_value(user_object, "hand");
        if (user_hand_json->type != JSON_OBJECT) {
            // TODO: Fail
        }

        int progress_hand = 1;
        if (user_action == USER_HIT) {
            deal_card(table_object, user_hand_json);

            if (tally_hand(user_hand_json) <= 21) {
                progress_hand = 0;
            }
        } else if (user_action == USER_STAND) {
            // TODO: Stand behavior (if any)
        } else {
            printf("Invalid user action when progressing blackjack hand\n");
            progress_hand = 0;
        }

        struct json_value *res_json = create_object_json_value();
        if (res_json->type != JSON_OBJECT) {
            // TODO: Fail
        }

        struct json_object *res_object = (struct json_object *)(res_json->data);

        json_object_add_value(&res_object, "users", users_json);

        // TODO: Error checking
        struct json_value *res_dealer_hand_json = create_array_json_value();
        if (res_dealer_hand_json->type != JSON_ARRAY) {
            // TODO: Fail
        }

        json_array_add_value((struct json_array **)(&res_dealer_hand_json->data), ((struct json_array *)(dealer_hand_json->data))->values[1]);

        // TODO: Error checking
        json_object_add_value(&res_object, "dealerCards", res_dealer_hand_json);

        // TODO: Error checking
        res = json_value_to_string(res_json);

        if (!progress_hand) {
            goto out;
        }
    } else if (hand_progress == num_users) {
        // Dealer's turn
        // TODO: Error checking
        struct json_value *dealer_json = json_object_get_value(table_object, "dealer");
        if (dealer_json->type != JSON_OBJECT) {
            // TODO: Fail
        }

        // TODO: Error checking
        struct json_value *dealer_hand_json = json_object_get_value((struct json_object *)(dealer_json->data), "hand");
        if (dealer_hand_json->type != JSON_ARRAY) {
            // TODO: Fail
        }

        int dealer_sum = tally_hand(dealer_hand_json);

        while (dealer_sum < 17) {
            deal_card(table_object, dealer_hand_json);

            dealer_sum = tally_hand(dealer_hand_json);
        }

        struct json_value *res_json = create_object_json_value();
        if (res_json->type != JSON_OBJECT) {
            // TODO: Fail
        }

        struct json_object *res_object = (struct json_object *)(res_json->data);

        // TODO: Error checking
        json_object_add_value(&res_object, "users", users_json);

        // TODO: Error checking
        json_object_add_value(&res_object, "dealerCards", dealer_hand_json);

        // TODO: Error checking
        res = json_value_to_string(res_json);
    } else {
        // Hand is over

        int dealer_value = tally_hand(dealer_hand_json);
        if (dealer_value > 21) {
            dealer_value = 0;
        }

        for (int i = 0; i < num_users; ++i) {
            struct json_value *tmp_user_json = users_array->values[i];
            if (tmp_user_json->type != JSON_OBJECT) {
                // TODO: Fail
            }

            struct json_value *tmp_user_hand_json = json_object_get_value((struct json_object *)(tmp_user_json->data), "hand");

            int user_value = tally_hand(tmp_user_hand_json);
            if (user_value > 21) {
                user_value = 0;
            }

            // TODO: Error checking
            struct json_value *tmp_user_id_json = json_object_get_value((struct json_object *)(tmp_user_json->data), "id");
            if (tmp_user_id_json->type != JSON_STRING) {
                // TODO: Fail
            }

            char *tmp_user_id = (char *)(tmp_user_id_json->data);

            // TODO: Error checking
            struct json_value *tmp_user_bet_json = json_object_get_value((struct json_object *)(tmp_user_json->data), "bet");
            if (tmp_user_bet_json->type != JSON_NUMBER) {
                // TODO: Fail
            }

            int tmp_user_bet = (int)(((struct json_number *)(tmp_user_bet_json->data))->integer);

            // TODO: Store user cash somewhere
            if (user_value > dealer_value) {
                printf("%s receives %d\n", tmp_user_id, tmp_user_bet);
            } else if (user_value < dealer_value) {
                printf("%s loses %d\n", tmp_user_id, tmp_user_bet);
            } else {
                if (!user_value) {
                    printf("%s loses %d\n", tmp_user_id, tmp_user_bet);
                } else {
                    printf("%s keeps %d\n", tmp_user_id, tmp_user_bet);
                }
            }
        }

        // TODO: Error checking on these
        json_object_remove_value(&table_object, "hand-progress");
        json_object_remove_value(&table_object, "deck");
        json_object_remove_value((struct json_object **)(&dealer_json->data), "hand");

        for (int i = 0; i < num_users; ++i) {
            struct json_value *tmp_user_json = users_array->values[i];

            // TODO: Error checking
            json_object_remove_value((struct json_object **)(&tmp_user_json->data), "hand");
        }

        // TODO: Construct a res

        goto out;
    }

    ++hand_progress;

    ((struct json_number *)(hand_progress_json->data))->integer = hand_progress;

out:
    // TODO: Destroy all JSON objects created at the top of this function

    return res;
}
