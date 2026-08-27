#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../include/server/games/blackjack/blackjack.h"

#include "../../../include/server/database/database.h"
#include "../../../include/server/utils/json_handler.h"

char *blackjack_place_bet(struct database_mappings *db, char *data) {
    // TODO: Error checking
    struct json_object *data_json = json_parse_string(data);

    // TODO: Error checking
    struct json_value *user_id_json = json_find_entry(data_json, "userID");
    char *user_id = user_id_json->str_val;

    // TODO: Error checking
    struct json_value *table_id_json = json_find_entry(data_json, "tableID");
    char *table_id = table_id_json->str_val;

    // TODO: Should be a number value once that is implemented
    struct json_value *bet_json = json_find_entry(data_json, "bet");
    char *bet = bet_json->str_val;

    // TODO: Error checking
    blackjack_join_table(db, data);

    struct database_entry *found_entry = db_find(db, table_id);

    if (found_entry == NULL) {
        // TODO: Return a JSON error
        printf("blackjack error: db entry is still null\n");
    }

    struct json_object *entry_json = json_parse_string((char *)found_entry->data_ptr);

    // TODO: Correlate bets to users and add support for more than one bet
    struct json_value *entry_bets_json = json_find_entry(entry_json, "bets");
    if (!entry_bets_json) {
        struct json_value *bets_val = (struct json_value *)malloc(sizeof(struct json_value));
        bets_val->str_val = bet;
        bets_val->type = JSON_STRING;

        json_add_entry(entry_json, "bets", bets_val);
    } else {
        entry_bets_json->str_val = bet;
    }

    free(found_entry->data_ptr);
    found_entry->data_ptr = json_to_string(entry_json);

    return data;
}
