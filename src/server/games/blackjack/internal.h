#ifndef BLACKJACK_INTERNAL_H
#define BLACKJACK_INTERNAL_H

#define USER_NO_ACTION 0
#define USER_HIT 1
#define USER_STAND 2

extern const int card_values[13];

int blackjack_tally_hand(int *hand, int hand_count);

#endif // BLACKJACK_INTERNAL_H
