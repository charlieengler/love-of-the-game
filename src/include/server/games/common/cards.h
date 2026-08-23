#ifndef CARDS_H
#define CARDS_H

#define NUM_CARDS 56

#define JOKER 0

#define ONE_DIAMOND 1
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
#define ACE_DIAMOND 14

#define ONE_CLUB 15
#define TWO_CLUB 16
#define THREE_CLUB 17
#define FOUR_CLUB 18
#define FIVE_CLUB 19
#define SIX_CLUB 20
#define SEVEN_CLUB 21
#define EIGHT_CLUB 22
#define NINE_CLUB 23
#define TEN_CLUB 24
#define JACK_CLUB 25
#define QUEEN_CLUB 26
#define KING_CLUB 27
#define ACE_CLUB 28

#define ONE_HEART 29
#define TWO_HEART 30
#define THREE_HEART 31
#define FOUR_HEART 32
#define FIVE_HEART 33
#define SIX_HEART 34
#define SEVEN_HEART 35
#define EIGHT_HEART 36
#define NINE_HEART 37
#define TEN_HEART 38
#define JACK_HEART 39
#define QUEEN_HEART 40
#define KING_HEART 41
#define ACE_HEART 42

#define ONE_SPADE 43
#define TWO_SPADE 44
#define THREE_SPADE 45
#define FOUR_SPADE 46
#define FIVE_SPADE 47
#define SIX_SPADE 48
#define SEVEN_SPADE 49
#define EIGHT_SPADE 50
#define NINE_SPADE 51
#define TEN_SPADE 52
#define JACK_SPADE 53
#define QUEEN_SPADE 54
#define KING_SPADE 55
#define ACE_SPADE 56

int *get_shuffled_deck(char num_jokers);
char *get_card_string(int card);
char *get_card_index_string(int *cards, int cardsSize);

#endif // CARDS_H
