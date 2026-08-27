#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../include/server/games/blackjack/blackjack.h"

#include "../../../include/server/database/database.h"
#include "../../../include/server/utils/json_handler.h"

char *blackjack_join_table(struct database_mappings *db, char *data) {
    // TODO: Error checking
    struct json_object *parsed_data = json_parse_string(data);

    // TODO: Error checking
    struct json_value *user_id_json = json_find_entry(parsed_data, "userID");
    char *user_id = user_id_json->str_val;

    // TODO: Error checking
    struct json_value *table_id_json = json_find_entry(parsed_data, "tableID");
    char *table_id = table_id_json->str_val;

    struct database_entry *found_table = db_find(db, table_id);

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
        db_insert(db, new_table);

        found_table = db_find(db, table_id);
    }

    if (found_table == NULL) {
        // TODO: Return a JSON error
        printf("blackjack error: db entry is still null\n");
    }

    // TODO: Add the ability to join an existing table

    printf("Joined table %s as %s\n", table_id, user_id);

    return data;
}
