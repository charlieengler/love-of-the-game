#include <stdio.h>

#include "./internal.h"

#include "../../../include/server/games/blackjack/blackjack.h"

#include "../../../include/server/games/common/cards.h"

#include "../../../include/server/database/database.h"
#include "../../../include/server/utils/json_api.h"

const int card_values[13] = {0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};

int deal_card(struct json_object *table_object, struct json_value *user_hand_json) {
    struct json_value *deck_json = json_object_get_value(table_object, "deck");

    if (!deck_json) {
        // TODO: Error checking
        deck_json = create_array_json_value();
        if (deck_json->type != JSON_ARRAY) {
            // TODO: Fail
        }

        json_object_add_value(&table_object, "deck", deck_json);
    }

    if (deck_json->type != JSON_ARRAY) {
        // TODO: Fail
    }

    struct json_array *deck_array = (struct json_array *)(deck_json->data);

    if (deck_array->length == 0) {
        // TODO: Support multiple decks in the table card pool
        int num_cards = NUM_CARDS;
        int *shuffled_deck = get_shuffled_deck(0);

        for (int i = 0; i < num_cards; ++i) {
            // TODO: Error checking
            struct json_value *card_json = create_number_json_value(shuffled_deck[i], 0, 0, JSON_INTEGER);
            if (card_json->type != JSON_NUMBER) {
                // TODO: Fail
            }

            // TODO: Error checking
            json_array_add_value(&deck_array, card_json);
        }
    }

    // TODO: Error checking
    struct json_value *popped_card_json = json_array_pop_value(deck_array);
    if (popped_card_json->type != JSON_NUMBER) {
        // TODO: Fail
    }

    // TODO: Error checking
    int selected_card = (int)(((struct json_number *)(popped_card_json->data))->integer);

    if (user_hand_json->type != JSON_ARRAY) {
        // TODO: Fail
    }

    struct json_array *user_hand_arr = (struct json_array *)(user_hand_json->data);

    json_array_add_value(&user_hand_arr, popped_card_json);

    return selected_card;
}

int tally_hand(struct json_value *user_hand_json) {
    if (user_hand_json->type != JSON_ARRAY) {
        // TODO: Fail
    }

    struct json_array *user_hand_arr = (struct json_array *)(user_hand_json->data);

    int sum = 0;
    int soft = 0;

    for (int i = 0; i < user_hand_arr->length; ++i) {
        struct json_value *card_json = user_hand_arr->values[i];
        if (card_json->type != JSON_NUMBER) {
            // TODO: Fail
        }

        int card = (int)(((struct json_number *)(card_json->data))->integer);
        int card_value = card_values[card % 13];

        if (card_value == 0) {
            if (sum > 10) {
                card_value = 1;
            } else {
                card_value = 11;
                soft = 1;
            }
        }

        if (soft && sum + card_value > 21) {
            soft = 0;

            sum -= 10;
        }

        sum += card_value;
    }

    return sum;
}
