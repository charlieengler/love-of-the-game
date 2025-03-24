#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../include/server/games/blackjack/blackjack.h"

#include "../../../include/server/utils/json_handler.h"
#include "../../../include/server/database/database.h"

char *blackjack_place_bet(struct database_mappings *db, char *data) {
    struct database_entry *new_entry = (struct database_entry*)malloc(sizeof(struct database_entry));
    struct json_object *entry_json = json_initialize_object();

    struct json_value *entry_val = (struct json_value*)malloc(sizeof(struct json_value));
    entry_val->str_val = data;
    entry_val->type = JSON_FORMATTED_STRING;

    // TODO: Unique user ID for each user to store in the database
    json_add_entry(entry_json, "charlie-blackjack", entry_val);
    
    new_entry->key = (char*)calloc(strlen("charlie-blackjack") + 1, sizeof(char));
    strcpy(new_entry->key, "charlie-blackjack");
    new_entry->type = (char*)calloc(strlen("json") + 1, sizeof(char));
    strcpy(new_entry->type, "json");
    new_entry->data_ptr = (void*)json_to_string(entry_json);

    // TODO: Error checking
    db_insert(db, new_entry);

    // TODO: Error checking
    struct database_entry *found_entry = db_find(db, "charlie-blackjack");

    printf("Placed Bet: charlie-blackjack - %s\n", (char*)found_entry->data_ptr);

    return (char*)found_entry->data_ptr;
}