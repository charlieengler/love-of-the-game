#include <stdlib.h>
#include <string.h>

#include "../../include/server/database/database.h"
#include "../../include/server/games/games.h"

char *route_blackjack(char *function, char *data) {
    struct database_mappings *blackjack_tables_db = NULL;

    if (blackjack_tables_db == NULL) {
        // TODO: Error checking
        db_initialize(&blackjack_tables_db, "blackjack_tables");
    }

    struct database_mappings *blackjack_bets_db = NULL;

    if (blackjack_bets_db == NULL) {
        // TODO: Error checking
        db_initialize(&blackjack_bets_db, "blackjack_bets");
    }

    char *return_header = NULL;

    if (strstr(function, "join-table") != NULL) {
        // TODO: Check for success/failure on this function (returns void right now)
        return_header = blackjack_join_table(blackjack_tables_db, data);
    } else if (strstr(function, "bet") != NULL) {
        // TODO: Check for success/failure on this function (returns void right now)
        return_header = blackjack_place_bet(blackjack_bets_db, data);
    }

    db_save(blackjack_tables_db);
    db_save(blackjack_bets_db);

    // TODO: Return an error that the function couldn't be found if the header is still NULL at this point
    if (return_header == NULL) {
        // TODO: Better return header error
        const char *header_const = "{\"error\":\"Make me more descriptive\"}";
        return_header = (char *)calloc(strlen(header_const) + 1, sizeof(char));
        strcpy(return_header, header_const);
    }

    return return_header;
}

// TODO: Return the response to the route request
char *route_game(char *path, char *data) {
    char *route = NULL;
    if ((route = strstr(path, "blackjack/")) != NULL) {
        char *res = route_blackjack(route + strlen("blackjack/"), data);

        return res;
    }

    const char *header_const = "{\"error\":\"Unknown game\"}";
    char *return_header = (char *)calloc(strlen(header_const) + 1, sizeof(char));
    strcpy(return_header, header_const);

    return return_header;
}
