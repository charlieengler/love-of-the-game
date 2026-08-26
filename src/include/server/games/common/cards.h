#ifndef CARDS_H
#define CARDS_H

#define NUM_CARDS 52

#define ACE_DIAMOND 0
#define TWO_DIAMOND 1
#define THREE_DIAMOND 2
#define FOUR_DIAMOND 3
#define FIVE_DIAMOND 4
#define SIX_DIAMOND 5
#define SEVEN_DIAMOND 6
#define EIGHT_DIAMOND 7
#define NINE_DIAMOND 8
#define TEN_DIAMOND 9
#define JACK_DIAMOND 10
#define QUEEN_DIAMOND 11
#define KING_DIAMOND 12

#define ACE_CLUB 13
#define TWO_CLUB 14
#define THREE_CLUB 15
#define FOUR_CLUB 16
#define FIVE_CLUB 17
#define SIX_CLUB 18
#define SEVEN_CLUB 19
#define EIGHT_CLUB 20
#define NINE_CLUB 21
#define TEN_CLUB 22
#define JACK_CLUB 23
#define QUEEN_CLUB 24
#define KING_CLUB 25

#define ACE_HEART 26
#define TWO_HEART 27
#define THREE_HEART 28
#define FOUR_HEART 29
#define FIVE_HEART 30
#define SIX_HEART 31
#define SEVEN_HEART 32
#define EIGHT_HEART 33
#define NINE_HEART 34
#define TEN_HEART 35
#define JACK_HEART 36
#define QUEEN_HEART 37
#define KING_HEART 38

#define ACE_SPADE 39
#define TWO_SPADE 40
#define THREE_SPADE 41
#define FOUR_SPADE 42
#define FIVE_SPADE 43
#define SIX_SPADE 44
#define SEVEN_SPADE 45
#define EIGHT_SPADE 46
#define NINE_SPADE 47
#define TEN_SPADE 48
#define JACK_SPADE 49
#define QUEEN_SPADE 50
#define KING_SPADE 51

#define JOKER 52

int *get_shuffled_deck(char num_jokers);
char *get_card_string(int card);
char *get_card_index_string(int *cards, int num_cards);
int *parse_card_index_string(char *cards_string, int *num_cards);

#endif // CARDS_H
