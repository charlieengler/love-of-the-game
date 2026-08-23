#ifndef BLACKJACK_H
#define BLACKJACK_H

struct database_mappings;

char *blackjack_join_table(struct database_mappings *db, char *data);
char *blackjack_place_bet(struct database_mappings *db, char *data);
char *blackjack_deal_cards(struct database_mappings *db, char *data);

#endif // BLACKJACK_H
