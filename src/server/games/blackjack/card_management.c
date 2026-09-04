#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./internal.h"

#include "../../../include/server/games/blackjack/blackjack.h"
#include "../../../include/server/games/common/cards.h"

#include "../../../include/server/database/database.h"
#include "../../../include/server/utils/json_handler.h"

const int card_values[13] = {0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};

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

int blackjack_tally_hand(int *hand, int hand_count) {
    int sum = 0;
    int soft = 0;

    for (int i = 0; i < hand_count; ++i) {
        int current_val = card_values[hand[i] % 13];

        if (current_val == 0) {
            if (sum > 10) {
                current_val = 1;
            } else {
                current_val = 11;
                soft = 1;
            }
        }

        if (soft && sum + current_val > 21) {
            soft = 0;

            sum -= 10;
        }

        sum += current_val;
    }

    return sum;
}
