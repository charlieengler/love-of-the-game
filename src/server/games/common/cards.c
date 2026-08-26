#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../../include/server/games/common/cards.h"

const char *card_names[] = {"Ace", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Joker"};

const char *suit_names[] = {"Diamonds", "Clubs", "Hearts", "Spades"};

int *get_shuffled_deck(char num_jokers) {
    int *deck = malloc(sizeof(int) * (NUM_CARDS + num_jokers));

    for (int i = 0; i < NUM_CARDS; ++i) {
        deck[i] = i + 1;
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

char *get_card_index_string(int *cards, int cardsSize) {
    char *ret_str = calloc(cardsSize * 3, sizeof(char));
    char num_buf[3];

    sprintf(num_buf, "%d", cards[0]);

    strcpy(ret_str, num_buf);

    for (int i = 1; i < cardsSize; ++i) {
        sprintf(num_buf, ",%d", cards[i]);

        strcat(ret_str, num_buf);
    }

    return ret_str;
}
