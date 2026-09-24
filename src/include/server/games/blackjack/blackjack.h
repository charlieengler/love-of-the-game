#ifndef BLACKJACK_H
#define BLACKJACK_H

struct json_value;

char *blackjack_join_table(struct json_value *db_json, char *req);
char *blackjack_place_bet(struct json_value *db_json, char *req);
char *blackjack_progress_hand(struct json_value *db_json, char *req, int user_action);

#endif // BLACKJACK_H
