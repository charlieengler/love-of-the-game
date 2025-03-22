#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../include/server/games/blackjack/blackjack.h"

#include "../../../include/server/utils/json_handler.h"
#include "../../../include/server/database/database.h"

char *blackjack_place_bet(struct database_mappings *db, char *data) {
    struct database_entry *new_entry = (struct database_entry*)malloc(sizeof(struct database_entry));
    // TODO: Unique user ID for each user to store in the database
    new_entry->key = (char*)calloc(strlen("charlie-blackjack") + 1, sizeof(char));
    strcpy(new_entry->key, "charlie-blackjack");
    new_entry->type = (char*)calloc(strlen("json") + 1, sizeof(char));
    strcpy(new_entry->type, "json");
    new_entry->data_ptr = (void*)data;

    // TODO: Error checking
    db_insert(db, new_entry);

    // TODO: Error checking
    struct database_entry *found_entry = db_find(db, "charlie-blackjack");

    printf("Placed Bet: charlie-blackjack - %s\n", (char*)found_entry->data_ptr);
    
    struct json_object *return_object = json_initialize_object();

    // TODO: Error handling
    json_add_entry(return_object, "success", "Bet Placed");

    // TODO: This needs to be freed at some point
    char *return_string = json_to_string(return_object);

    return return_string;
}