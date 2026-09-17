#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "internal.h"

#include "../../../include/server/games/blackjack/blackjack.h"

#include "../../../include/server/database/database.h"
#include "../../../include/server/utils/json_handler.h"
#include "../../../include/server/utils/strings.h"

int add_blank_bet(struct database_mappings *db, char *data) {
    // TODO: Error checking
    struct json_object *data_json = json_parse_string(data);

    // TODO: Error checking
    struct json_value *table_id_json = json_find_entry(data_json, "tableID");
    char *table_id = table_id_json->str_val;

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
        entry_bets_json = (struct json_value *)malloc(sizeof(struct json_value));
        entry_bets_json->str_val = (char *)malloc(sizeof(char));
        entry_bets_json->str_val[0] = '\0';
        entry_bets_json->type = JSON_STRING;

        json_add_entry(entry_json, "bets", entry_bets_json);
    }

    char *new_bets = (char *)malloc(strlen(entry_bets_json->str_val) + strlen("0") + 2);
    strcpy(new_bets, entry_bets_json->str_val);
    if (strlen(entry_bets_json->str_val)) {
        strcat(new_bets, ",");
    }
    strcat(new_bets, "0");

    new_bets[strlen(entry_bets_json->str_val) + strlen("0") + 1] = '\0';

    free(entry_bets_json->str_val);

    entry_bets_json->str_val = new_bets;

    free(found_entry->data_ptr);
    found_entry->data_ptr = json_to_string(entry_json);

    return 0;
}

char *blackjack_place_bet(struct database_mappings *db, char *data) {
    // TODO: Error checking
    struct json_object *data_json = json_parse_string(data);

    // TODO: Error checking
    struct json_value *table_id_json = json_find_entry(data_json, "tableID");
    char *table_id = table_id_json->str_val;

    // TODO: Error checking
    struct json_value *user_id_json = json_find_entry(data_json, "userID");
    char *user_id = user_id_json->str_val;

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

    struct json_value *entry_users_json = json_find_entry(entry_json, "users");
    char *entry_users = entry_users_json->str_val;

    int num_users = 0;
    char **users = csv_to_string_array(entry_users, &num_users);

    // TODO: Correlate bets to users and add support for more than one bet
    struct json_value *entry_bets_json = json_find_entry(entry_json, "bets");
    if (!entry_bets_json) {
        printf("Entry bets JSON was null when placing bet\n");

        return data;
    }

    int num_bets = 0;
    int *bets = csv_to_int_array(entry_bets_json->str_val, &num_bets);

    if (num_bets != num_users) {
        printf("Number of bets didn't equal number of users when placing bet\n");

        return data;
    }

    for (int i = 0; i < num_users; ++i) {
        if (!strcmp(user_id, users[i])) {
            bets[i] = atoi(bet);

            break;
        }
    }

    free(entry_bets_json->str_val);

    entry_bets_json->str_val = int_array_to_csv(bets, num_bets);

    free(found_entry->data_ptr);
    found_entry->data_ptr = json_to_string(entry_json);

    return data;
}
