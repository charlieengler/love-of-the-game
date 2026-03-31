#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../include/server/games/blackjack/blackjack.h"

#include "../../../include/server/utils/json_handler.h"
#include "../../../include/server/database/database.h"

char *blackjack_place_bet(struct database_mappings *db, char *data) {
    // TODO: Error checking
    struct database_entry *found_entry = db_find(db, "charlie");

    if(found_entry == NULL) {
        printf("blackjack error: could not find entry %s in database\n", "charlie");

        struct database_entry *new_entry = (struct database_entry*)malloc(sizeof(struct database_entry));
        struct json_object *entry_json = json_initialize_object();

        struct json_value *entry_val = (struct json_value*)malloc(sizeof(struct json_value));
        entry_val->str_val = data;
        entry_val->type = JSON_FORMATTED_STRING;

        // TODO: Unique user ID for each user to store in the database
        json_add_entry(entry_json, "charlie-blackjack-bet", entry_val);

        new_entry->key = (char*)calloc(strlen("charlie") + 1, sizeof(char));
        strcpy(new_entry->key, "charlie");
        new_entry->type = DB_JSON;
        new_entry->data_ptr = (void*)json_to_string(entry_json);

        // TODO: Error checking
        db_insert(db, new_entry);

        found_entry = db_find(db, "charlie");
    }

    if(found_entry == NULL) {
        printf("blackjack error: db entry is still null\n");
    }

    printf("Placed Bet: charlie - %s\n", (char*)found_entry->data_ptr);

    return (char*)found_entry->data_ptr;
}
