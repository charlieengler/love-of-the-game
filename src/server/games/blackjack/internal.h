#ifndef BLACKJACK_INTERNAL_H
#define BLACKJACK_INTERNAL_H

struct json_value;
struct json_object;

#define USER_NO_ACTION 0
#define USER_HIT 1
#define USER_STAND 2

extern const int card_values[13];

int tally_hand(struct json_value *hand_json);
int deal_card(struct json_object *table_object, struct json_value *user_hand_json);

#endif // BLACKJACK_INTERNAL_H
