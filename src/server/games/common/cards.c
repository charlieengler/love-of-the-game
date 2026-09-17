#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../../include/server/games/common/cards.h"
#include "../../../include/server/utils/strings.h"

const char *card_names[] = {"Ace", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Joker"};

const char *suit_names[] = {"Diamonds", "Clubs", "Hearts", "Spades"};

int *get_shuffled_deck(char num_jokers) {
    int *deck = malloc(sizeof(int) * (NUM_CARDS + num_jokers));

    for (int i = 0; i < NUM_CARDS; ++i) {
        deck[i] = i;
    }

    for (int i = NUM_CARDS; i < NUM_CARDS + num_jokers; ++i) {
        deck[i] = JOKER;
    }

    srand(time(NULL));

    for (int i = NUM_CARDS - 1; i > 0; --i) {
        int j = rand() % (i + 1);

        int tmp = deck[i];
        deck[i] = deck[j];
        deck[j] = tmp;
    }

    for (int i = NUM_CARDS; i < NUM_CARDS + num_jokers; ++i) {
        int j = rand() % (NUM_CARDS + num_jokers);

        int tmp = deck[j];
        deck[j] = deck[i];
        deck[i] = tmp;
    }

    return deck;
}

char *get_card_string(int card) {
    char *ret_str;

    if (card == JOKER) {
        ret_str = calloc(strlen("Joker") + 1, sizeof(char));
        strcpy(ret_str, "Joker");

        return ret_str;
    }

    const char *card_name = card_names[(card - 1) % 13];
    const char *of = " of ";
    const char *card_suit = suit_names[(card - 1) % 4];

    ret_str = calloc(strlen(card_name) + strlen(of) + strlen(card_suit) + 1, sizeof(char));
    strcpy(ret_str, card_name);
    strcat(ret_str, of);
    strcat(ret_str, card_suit);

    return ret_str;
}

char *get_card_index_string(int *cards, int num_cards) { return int_array_to_csv(cards, num_cards); }

int *parse_card_index_string(char *cards_string, int *num_cards) { return csv_to_int_array(cards_string, num_cards); }
