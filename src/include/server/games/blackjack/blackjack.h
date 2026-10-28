#ifndef BLACKJACK_H
#define BLACKJACK_H

struct json_value;

int blackjack_join_table(struct json_value *db_json, char *req, char **send_buffer);
int blackjack_status_update(struct json_value *db_json, char *req, char **send_buffer);
int blackjack_place_bet(struct json_value *db_json, char *req, char **send_buffer);
int blackjack_progress_hand(struct json_value *db_json, char *req, int user_action, char **send_buffer);

#endif // BLACKJACK_H
