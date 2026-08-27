#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../include/server/games/blackjack/blackjack.h"
#include "../../../include/server/games/common/cards.h"

#include "../../../include/server/database/database.h"
#include "../../../include/server/utils/json_handler.h"

char *blackjack_join_table(struct database_mappings *db, char *data) {
    // TODO: Error checking
    struct json_object *parsed_data = json_parse_string(data);

    // TODO: Error checking
    struct json_value *user_id_json = json_find_entry(parsed_data, "userID");
    char *user_id = user_id_json->str_val;

    // TODO: Error checking
    struct json_value *table_id_json = json_find_entry(parsed_data, "tableID");
    char *table_id = table_id_json->str_val;

    struct database_entry *found_table = db_find(db, table_id);

    if (found_table == NULL) {
        printf("blackjack error: could not find entry %s in database\n", table_id);

        struct database_entry *new_table = (struct database_entry *)malloc(sizeof(struct database_entry));
        struct json_object *new_table_json = json_initialize_object();

        struct json_value *new_table_val = (struct json_value *)malloc(sizeof(struct json_value));
        new_table_val->str_val = user_id;
        new_table_val->type = JSON_STRING;

        // TODO: Unique user ID for each user to store in the database
        json_add_entry(new_table_json, "users", new_table_val);

        new_table->key = (char *)calloc(strlen(table_id) + 1, sizeof(char));
        strcpy(new_table->key, table_id);
        new_table->type = DB_STRING;
        new_table->data_ptr = (void *)json_to_string(new_table_json);

        // TODO: Error checking
        db_insert(db, new_table);

        found_table = db_find(db, table_id);
    }

    if (found_table == NULL) {
        // TODO: Return a JSON error
        printf("blackjack error: db entry is still null\n");
    }

    printf("Joined table %s as %s\n", table_id, user_id);

    return data;
}

char *blackjack_place_bet(struct database_mappings *db, char *data) {
    // TODO: Error checking
    struct json_object *data_json = json_parse_string(data);

    // TODO: Error checking
    struct json_value *user_id_json = json_find_entry(data_json, "userID");
    char *user_id = user_id_json->str_val;

    // TODO: Error checking
    struct json_value *table_id_json = json_find_entry(data_json, "tableID");
    char *table_id = table_id_json->str_val;

    // TODO: Should be a number value once that is implemented
    struct json_value *bet_json = json_find_entry(data_json, "bet");
    char *bet = bet_json->str_val;

    // TODO: Error checking
    blackjack_join_table(db, data);

    struct database_entry *found_entry = db_find(db, table_id);

    if (found_entry == NULL) {
        // TODO: Return a JSON error
        printf("blackjack error: db entry is still null\n");
    }

    struct json_object *entry_json = json_parse_string((char *)found_entry->data_ptr);

    struct json_value *entry_bets_json = json_find_entry(entry_json, "bets");
    if (!entry_bets_json) {
        struct json_value *bets_val = (struct json_value *)malloc(sizeof(struct json_value));
        bets_val->str_val = bet;
        bets_val->type = JSON_STRING;

        json_add_entry(entry_json, "bets", bets_val);
    } else {
        entry_bets_json->str_val = bet;
    }

    free(found_entry->data_ptr);
    found_entry->data_ptr = json_to_string(entry_json);

    return data;
}

int blackjack_deal_card(struct database_mappings *db, char *data) {
    // TODO: Error checking
    struct json_object *data_json = json_parse_string(data);

    // TODO: Error checking
    struct json_value *table_id_json = json_find_entry(data_json, "tableID");
    char *table_id = table_id_json->str_val;

    // TODO: Error checking
    blackjack_join_table(db, data);

    struct database_entry *found_entry = db_find(db, table_id);

    if (found_entry == NULL) {
        // TODO: Return a JSON error
        printf("blackjack error: db entry is still null\n");
    }

    struct json_object *entry_json = json_parse_string((char *)found_entry->data_ptr);

    struct json_value *entry_cards_json = json_find_entry(entry_json, "cards");
    if (!entry_cards_json) {
        int *shuffled_deck = get_shuffled_deck(0);

        struct json_value *cards_val = (struct json_value *)malloc(sizeof(struct json_value));
        cards_val->str_val = get_card_index_string(shuffled_deck, NUM_CARDS);
        cards_val->type = JSON_STRING;

        free(shuffled_deck);

        json_add_entry(entry_json, "cards", cards_val);

        free(found_entry->data_ptr);
        found_entry->data_ptr = json_to_string(entry_json);

        entry_cards_json = json_find_entry(entry_json, "cards");
    }

    int num_cards = 0;
    int *cards_array = parse_card_index_string(entry_cards_json->str_val, &num_cards);

    int selected_card = cards_array[0];

    free(entry_cards_json->str_val);
    if (num_cards > 1) {
        entry_cards_json->str_val = get_card_index_string(&cards_array[1], num_cards - 1);
    } else {
        entry_cards_json->str_val = get_card_index_string(get_shuffled_deck(0), NUM_CARDS);
    }

    free(cards_array);

    free(found_entry->data_ptr);
    found_entry->data_ptr = json_to_string(entry_json);

    db_save(db);

    return selected_card;
}

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
