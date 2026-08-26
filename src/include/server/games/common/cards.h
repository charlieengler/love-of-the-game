#ifndef CARDS_H
#define CARDS_H

#define NUM_CARDS 55

#define JOKER 0

#define ACE_DIAMOND 1
#define TWO_DIAMOND 2
#define THREE_DIAMOND 3
#define FOUR_DIAMOND 4
#define FIVE_DIAMOND 5
#define SIX_DIAMOND 6
#define SEVEN_DIAMOND 7
#define EIGHT_DIAMOND 8
#define NINE_DIAMOND 9
#define TEN_DIAMOND 10
#define JACK_DIAMOND 11
#define QUEEN_DIAMOND 12
#define KING_DIAMOND 13

#define ACE_CLUB 14
#define TWO_CLUB 15
#define THREE_CLUB 16
#define FOUR_CLUB 17
#define FIVE_CLUB 18
#define SIX_CLUB 19
#define SEVEN_CLUB 20
#define EIGHT_CLUB 21
#define NINE_CLUB 22
#define TEN_CLUB 23
#define JACK_CLUB 24
#define QUEEN_CLUB 25
#define KING_CLUB 26

#define ACE_HEART 27
#define TWO_HEART 28
#define THREE_HEART 29
#define FOUR_HEART 30
#define FIVE_HEART 31
#define SIX_HEART 32
#define SEVEN_HEART 33
#define EIGHT_HEART 34
#define NINE_HEART 35
#define TEN_HEART 36
#define JACK_HEART 37
#define QUEEN_HEART 38
#define KING_HEART 39

#define ACE_SPADE 40
#define ONE_SPADE 41
#define TWO_SPADE 42
#define THREE_SPADE 43
#define FOUR_SPADE 44
#define FIVE_SPADE 45
#define SIX_SPADE 46
#define SEVEN_SPADE 47
#define EIGHT_SPADE 48
#define NINE_SPADE 49
#define TEN_SPADE 50
#define JACK_SPADE 51
#define QUEEN_SPADE 52
#define KING_SPADE 53

int *get_shuffled_deck(char num_jokers);
char *get_card_string(int card);
char *get_card_index_string(int *cards, int cardsSize);

#endif // CARDS_H
