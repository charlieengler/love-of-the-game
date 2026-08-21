#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../include/server/games/blackjack/blackjack.h"

#include "../../../include/server/database/database.h"
#include "../../../include/server/utils/json_handler.h"

char *blackjack_join_table(struct database_mappings *tables_db, char *data) {
    struct json_object *parsed_data = json_parse_string(data);

    struct json_value *user_id_json = json_find_entry(parsed_data, "userID");
    char *user_id = user_id_json->str_val;

    struct json_value *table_id_json = json_find_entry(parsed_data, "tableID");
    char *table_id = table_id_json->str_val;

    // TODO: Error checking
    struct database_entry *found_table = db_find(tables_db, table_id);

    if (found_table == NULL) {
        printf("blackjack error: could not find entry %s in database\n", table_id);

        struct database_entry *new_table = (struct database_entry *)malloc(sizeof(struct database_entry));
        struct json_object *new_table_json = json_initialize_object();

        struct json_value *new_table_val = (struct json_value *)malloc(sizeof(struct json_value));
        new_table_val->str_val = user_id;
        new_table_val->type = JSON_STRING;

        // TODO: Unique user ID for each user to store in the database
        json_add_entry(new_table_json, "users", new_table_val);

        new_table->key = (char *)calloc(strlen(table_id) + 1, sizeof(char));
        strcpy(new_table->key, table_id);
        new_table->type = DB_STRING;
        new_table->data_ptr = (void *)json_to_string(new_table_json);

        // TODO: Error checking
        db_insert(tables_db, new_table);

        found_table = db_find(tables_db, table_id);
    }

    if (found_table == NULL) {
        printf("blackjack error: db entry is still null\n");
    }

    printf("Joined table %s as %s - %s\n", table_id, user_id, (char *)found_table->data_ptr);

    json_destroy_object(parsed_data);

    return (char *)found_table->data_ptr;
}

char *blackjack_place_bet(struct database_mappings *db, char *data) {
    // TODO: Error checking
    struct database_entry *found_entry = db_find(db, "charlie");

    if (found_entry == NULL) {
        printf("blackjack error: could not find entry %s in database\n", "charlie");

        struct database_entry *new_entry = (struct database_entry *)malloc(sizeof(struct database_entry));
        struct json_object *entry_json = json_initialize_object();

        struct json_value *entry_val = (struct json_value *)malloc(sizeof(struct json_value));
        entry_val->str_val = data;
        entry_val->type = JSON_FORMATTED_STRING;

        // TODO: Unique user ID for each user to store in the database
        json_add_entry(entry_json, "blackjack-bet", entry_val);

        // TODO: This should not be hard coded either
        new_entry->key = (char *)calloc(strlen("charlie") + 1, sizeof(char));
        strcpy(new_entry->key, "charlie");
        new_entry->type = DB_STRING;
        new_entry->data_ptr = (void *)json_to_string(entry_json);

        // TODO: Error checking
        db_insert(db, new_entry);

        found_entry = db_find(db, "charlie");
    }

    if (found_entry == NULL) {
        printf("blackjack error: db entry is still null\n");
    }

    struct json_object *entry_json = json_initialize_object();

    struct json_value *entry_val = (struct json_value *)malloc(sizeof(struct json_value));
    entry_val->str_val = data;
    entry_val->type = JSON_FORMATTED_STRING;

    // TODO: Unique user ID for each user to store in the database
    json_add_entry(entry_json, "charlie-blackjack-bet", entry_val);

    found_entry->type = DB_STRING;
    free(found_entry->data_ptr);
    found_entry->data_ptr = (void *)json_to_string(entry_json);

    printf("Placed Bet: %s - %s\n", (char *)found_entry->key, (char *)found_entry->data_ptr);

    // TODO: This does not account for different data types and assumes it's a string underneath
    char *return_header = (char *)calloc(strlen(found_entry->data_ptr), sizeof(char));
    strcpy(return_header, found_entry->data_ptr);

    return return_header;
}
