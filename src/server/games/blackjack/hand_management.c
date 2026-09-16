#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./internal.h"

#include "../../../include/server/games/blackjack/blackjack.h"
#include "../../../include/server/games/common/cards.h"

#include "../../../include/server/database/database.h"
#include "../../../include/server/utils/json_handler.h"

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
char *blackjack_progress_hand(struct database_mappings *db, char *data, int user_action) {
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

    int hand_progress = -1;
    // TODO: Error checking
    struct json_value *hand_progress_json = json_add_string_entry(entry_json, "hand_progress", "-1");

    free(found_entry->data_ptr);
    found_entry->data_ptr = json_to_string(entry_json);

    hand_progress = atoi(hand_progress_json->str_val);

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
    if (hand_progress == -1) {
        int *user_cards = (int *)malloc(2 * num_users * sizeof(int));
        for (int i = 0; i < num_users * 2; i += 2) {
            int new_card = blackjack_deal_card(db, entry_json, data);

            user_cards[i] = new_card;
        }

        int *dealer_cards = (int *)malloc(2 * sizeof(int));
        dealer_cards[0] = blackjack_deal_card(db, entry_json, data);

        for (int i = 1; i < num_users * 2; i += 2) {
            int new_card = blackjack_deal_card(db, entry_json, data);

            user_cards[i] = new_card;
        }

        dealer_cards[1] = blackjack_deal_card(db, entry_json, data);

        struct json_object *return_json = json_initialize_object();

        json_add_entry(return_json, "users", users_json);

        json_add_string_entry(return_json, "userCards", get_card_index_string(user_cards, 2 * num_users));

        int *user_card_counts = (int *)malloc(num_users * sizeof(int));

        for (int i = 0; i < num_users; ++i) {
            user_card_counts[i] = 2;
        }

        char *card_count_str = generate_card_count_str(num_users, user_card_counts);

        json_add_string_entry(return_json, "userCardCounts", card_count_str);

        free(card_count_str);

        json_add_string_entry(return_json, "dealerCards", get_card_index_string(&dealer_cards[1], 1));

        return_data = json_to_string(return_json);

        free(return_json);

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

        struct json_value *dealer_cards_json_save = (struct json_value *)malloc(sizeof(struct json_value));
        dealer_cards_json_save->str_val = get_card_index_string(dealer_cards, 2);
        dealer_cards_json_save->type = JSON_STRING;

        json_add_entry(entry_json, "dealer-hand", dealer_cards_json_save);

        free(found_entry->data_ptr);
        found_entry->data_ptr = json_to_string(entry_json);
    } else if (hand_progress >= 0 && hand_progress < num_users) {
        // Turn for each player

        // TODO: Error checking
        struct json_value *user_id_json = json_find_entry(parsed_data, "userID");
        char *user_id = user_id_json->str_val;

        if (!strcmp(user_id, users[hand_progress])) {
            // TODO: Error stating that it's wrong user's turn
            printf("User ID did not match!\n");

            goto out;
        }

        char *user_hand_key = (char *)malloc((strlen(user_id) + strlen("-hand") + 1) * sizeof(char));
        strcpy(user_hand_key, user_id);
        strcat(user_hand_key, "-hand\0");
        struct json_value *user_hand_json = json_find_entry(entry_json, user_hand_key);
        free(user_hand_key);

        int num_user_cards;
        int *user_hand = parse_card_index_string(user_hand_json->str_val, &num_user_cards);

        int progress_hand = 1;
        if (user_action == USER_HIT) {
            int new_card = blackjack_deal_card(db, entry_json, data);

            int *new_hand = (int *)malloc((num_user_cards + 1) * sizeof(int));
            for (int i = 0; i < num_user_cards; ++i) {
                new_hand[i] = user_hand[i];
            }

            free(user_hand);

            new_hand[num_user_cards] = new_card;

            user_hand = new_hand;

            ++num_user_cards;

            if (blackjack_tally_hand(user_hand, num_user_cards) <= 21) {
                progress_hand = 0;
            }

            free(user_hand_json->str_val);

            user_hand_json->str_val = get_card_index_string(user_hand, num_user_cards);

            free(found_entry->data_ptr);
            found_entry->data_ptr = json_to_string(entry_json);
        } else if (user_action == USER_STAND) {
            // TODO: Stand behavior (if any)
        } else {
            printf("Invalid user action when progressing blackjack hand\n");
            progress_hand = 0;
        }

        struct json_object *return_json = json_initialize_object();

        json_add_entry(return_json, "users", users_json);

        int total_user_cards = 0;
        int *user_hand_counts = (int *)malloc(num_users * sizeof(int));
        int **user_hands = (int **)malloc(num_users * sizeof(int *));
        for (int i = 0; i < num_users; ++i) {
            char *user_hand_key = (char *)malloc((strlen(users[i]) + strlen("-hand") + 1) * sizeof(char));
            strcpy(user_hand_key, users[i]);
            strcat(user_hand_key, "-hand\0");

            struct json_value *user_hand_json = json_find_entry(entry_json, user_hand_key);

            free(user_hand_key);

            int num_cards_in_hand = 0;
            int *user_hand = parse_card_index_string(user_hand_json->str_val, &num_cards_in_hand);

            user_hand_counts[i] = num_cards_in_hand;
            total_user_cards += num_cards_in_hand;

            user_hands[i] = (int *)malloc(num_cards_in_hand * sizeof(int));
            for (int j = 0; j < num_cards_in_hand; ++j) {
                user_hands[i][j] = user_hand[j];
            }
        }

        int *user_cards = malloc(total_user_cards * sizeof(int));
        int k = 0;
        for (int i = 0; i < num_users; ++i) {
            for (int j = 0; j < user_hand_counts[i]; ++j) {
                user_cards[k] = user_hands[i][j];

                ++k;
            }
        }

        json_add_string_entry(return_json, "userCards", get_card_index_string(user_cards, total_user_cards));

        char *card_count_str = generate_card_count_str(num_users, user_hand_counts);

        json_add_string_entry(return_json, "userCardCounts", card_count_str);

        free(card_count_str);

        struct json_value *dealer_hand_json = json_find_entry(entry_json, "dealer-hand");
        int num_dealer_cards;
        int *dealer_cards = (int *)malloc(2 * sizeof(int));
        dealer_cards = parse_card_index_string(dealer_hand_json->str_val, &num_dealer_cards);

        json_add_string_entry(return_json, "dealerCards", get_card_index_string(&dealer_cards[1], 1));

        return_data = json_to_string(return_json);

        free(return_json);

        if (!progress_hand) {
            goto out;
        }
    } else if (hand_progress == num_users) {
        // Dealer's turn
        int total_user_cards = 0;
        int *user_hand_counts = (int *)malloc(num_users * sizeof(int));
        int **user_hands = (int **)malloc(num_users * sizeof(int *));
        for (int i = 0; i < num_users; ++i) {
            char *user_hand_key = (char *)malloc((strlen(users[i]) + strlen("-hand") + 1) * sizeof(char));
            strcpy(user_hand_key, users[i]);
            strcat(user_hand_key, "-hand\0");

            struct json_value *user_hand_json = json_find_entry(entry_json, user_hand_key);

            free(user_hand_key);

            int num_cards_in_hand = 0;
            int *user_hand = parse_card_index_string(user_hand_json->str_val, &num_cards_in_hand);

            user_hand_counts[i] = num_cards_in_hand;
            total_user_cards += num_cards_in_hand;

            user_hands[i] = (int *)malloc(num_cards_in_hand * sizeof(int));
            for (int j = 0; j < num_cards_in_hand; ++j) {
                user_hands[i][j] = user_hand[j];
            }
        }

        struct json_value *dealer_hand_json = json_find_entry(entry_json, "dealer-hand");

        int dealer_hand_count = 0;
        int *dealer_hand = parse_card_index_string(dealer_hand_json->str_val, &dealer_hand_count);

        int dealer_sum = blackjack_tally_hand(dealer_hand, dealer_hand_count);

        while (dealer_sum < 17) {
            int new_card = blackjack_deal_card(db, entry_json, data);

            int *new_hand = (int *)malloc((dealer_hand_count + 1) * sizeof(int));
            for (int i = 0; i < dealer_hand_count; ++i) {
                new_hand[i] = dealer_hand[i];
            }

            free(dealer_hand);

            new_hand[dealer_hand_count] = new_card;

            dealer_hand = new_hand;

            ++dealer_hand_count;

            dealer_sum = blackjack_tally_hand(dealer_hand, dealer_hand_count);
        }

        struct json_value *dealer_cards_json = json_find_entry(entry_json, "dealer-hand");

        free(dealer_cards_json->str_val);

        dealer_cards_json->str_val = get_card_index_string(dealer_hand, dealer_hand_count);

        free(found_entry->data_ptr);
        found_entry->data_ptr = json_to_string(entry_json);

        struct json_object *return_json = json_initialize_object();

        json_add_entry(return_json, "users", users_json);

        int *user_cards = malloc(total_user_cards * sizeof(int));
        int k = 0;
        for (int i = 0; i < num_users; ++i) {
            for (int j = 0; j < user_hand_counts[i]; ++j) {
                user_cards[k] = user_hands[i][j];

                ++k;
            }
        }

        json_add_string_entry(return_json, "userCards", get_card_index_string(user_cards, total_user_cards));

        char *card_count_str = generate_card_count_str(num_users, user_hand_counts);

        json_add_string_entry(return_json, "userCardCounts", card_count_str);

        free(card_count_str);

        json_add_entry(return_json, "dealerCards", dealer_cards_json);

        return_data = json_to_string(return_json);

        free(return_json);
    } else {
        // Hand is over
        // TODO: Implement me
    }

    hand_progress_json = json_find_entry(entry_json, "hand_progress");

    sprintf(hand_progress_json->str_val, "%d", hand_progress + 1);

out:
    free(found_entry->data_ptr);
    found_entry->data_ptr = json_to_string(entry_json);

    return return_data;
}
