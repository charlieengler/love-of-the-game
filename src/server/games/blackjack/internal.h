#ifndef BLACKJACK_INTERNAL_H
#define BLACKJACK_INTERNAL_H

struct database_mappings;
struct json_object;

#define USER_NO_ACTION 0
#define USER_HIT 1
#define USER_STAND 2

extern const int card_values[13];

int tally_hand(int *hand, int hand_count);
int deal_card(struct database_mappings *db, struct json_object *entry_json, char *data);
int add_blank_bet(struct database_mappings *db, char *data);

#endif // BLACKJACK_INTERNAL_H
