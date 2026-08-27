#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../include/server/games/blackjack/blackjack.h"
#include "../../../include/server/games/common/cards.h"

#include "../../../include/server/database/database.h"
#include "../../../include/server/utils/json_handler.h"

char *blackjack_progress_hand(struct database_mappings *db, char *data) {
    // TODO: Error checking
    struct json_object *parsed_data = json_parse_string(data);

    // TODO: Error checking
    struct json_value *table_id_json = json_find_entry(parsed_data, "tableID");
    char *table_id = table_id_json->str_val;

    struct database_entry *found_entry = db_find(db, table_id);

    if (found_entry == NULL) {
        printf("blackjack error: could not find entry %s in database when starting hand\n", table_id);

        return NULL;
    }

    struct json_object *entry_json = json_parse_string((char *)found_entry->data_ptr);

    int hand_progress = 0;
    struct json_value *hand_progress_json = json_find_entry(entry_json, "hand_progress");
    if (!hand_progress_json) {
        hand_progress_json = (struct json_value *)malloc(sizeof(struct json_value));
        hand_progress_json->str_val = (char *)malloc(2 * sizeof(char));
        strcpy(hand_progress_json->str_val, "0");
        hand_progress_json->type = JSON_STRING;

        json_add_entry(entry_json, "hand_progress", hand_progress_json);

        free(found_entry->data_ptr);
        found_entry->data_ptr = json_to_string(entry_json);
    } else {
        hand_progress = atoi(hand_progress_json->str_val);
    }

    sprintf(hand_progress_json->str_val, "%d", hand_progress + 1);

    free(found_entry->data_ptr);
    found_entry->data_ptr = json_to_string(entry_json);

    int num_users = 1;
    struct json_value *users_json = json_find_entry(entry_json, "users");
    char *users_string = (char *)malloc(strlen(users_json->str_val) * sizeof(char));
    strcpy(users_string, users_json->str_val);
    char *users_string_start = users_string;
    while (*users_string) {
        if (*users_string == ',') {
            ++num_users;
        }

        ++users_string;
    }

    char **users = (char **)malloc(num_users * sizeof(char *));
    char *current_user = users_string_start;
    users_string = users_string_start;
    int i = 0;
    while (*users_string) {
        if (*users_string == ',') {
            *users_string = '\0';

            users[i] = (char *)malloc(strlen(current_user) * sizeof(char));
            strcpy(users[i], current_user);

            current_user = users_string + 1;

            ++i;
        }

        ++users_string;
    }

    users[i] = (char *)malloc((strlen(current_user) + 1) * sizeof(char));
    strcpy(users[i], current_user);
    users[i][strlen(current_user)] = '\0';

    free(users_string_start);

    char *return_data = data;
    if (hand_progress == 0) {
        int *user_cards = (int *)malloc(2 * num_users * sizeof(int));
        for (int i = 0; i < num_users * 2; i += 2) {
            int new_card = blackjack_deal_card(db, data);

            user_cards[i] = new_card;
        }

        int *dealer_cards = (int *)malloc(2 * sizeof(int));
        dealer_cards[0] = blackjack_deal_card(db, data);

        for (int i = 1; i < num_users * 2; i += 2) {
            int new_card = blackjack_deal_card(db, data);

            user_cards[i] = new_card;
        }

        dealer_cards[1] = blackjack_deal_card(db, data);

        struct json_object *return_json = json_initialize_object();

        json_add_entry(return_json, "users", users_json);

        struct json_value *user_cards_json = (struct json_value *)malloc(sizeof(struct json_value));
        user_cards_json->str_val = get_card_index_string(user_cards, 2 * num_users);
        user_cards_json->type = JSON_STRING;

        json_add_entry(return_json, "userCards", user_cards_json);

        struct json_value *dealer_cards_json = (struct json_value *)malloc(sizeof(struct json_value));
        dealer_cards_json->str_val = get_card_index_string(&dealer_cards[1], 1);
        dealer_cards_json->type = JSON_STRING;

        json_add_entry(return_json, "dealerCards", dealer_cards_json);

        return_data = json_to_string(return_json);

        entry_json = json_parse_string((char *)found_entry->data_ptr);

        for (int i = 0; i < num_users; ++i) {
            struct json_value *user_hand_json = (struct json_value *)malloc(sizeof(struct json_value));
            user_hand_json->str_val = get_card_index_string(&user_cards[i * 2], 2);
            user_hand_json->type = JSON_STRING;

            char *user_hand_key = (char *)malloc((strlen(users[i]) + strlen("-hand") + 1) * sizeof(char));
            strcpy(user_hand_key, users[i]);
            strcat(user_hand_key, "-hand\0");

            json_add_entry(entry_json, user_hand_key, user_hand_json);
        }

        free(found_entry->data_ptr);
        found_entry->data_ptr = json_to_string(entry_json);
    }

    return return_data;
}
