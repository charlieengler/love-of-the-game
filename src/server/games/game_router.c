#include <stdio.h>
#include <string.h>

#include "../../include/server/games/games.h"
#include "../../include/server/database/database.h"

// TODO: Initilialize database for each service that requires it and search disk for previously stored instance of said databases
struct database_mappings *blackjack_db = NULL;

char *route_blackjack(char *function, char *data) {
    if(blackjack_db == NULL) {
        // TODO: Error checking
        db_initialize(&blackjack_db, "blackjack");
    }

    db_save(blackjack_db);

    if(strstr(function, "bet") != NULL) {
        // TODO: Check for success/failure on this function (returns void right now)
        return blackjack_place_bet(blackjack_db, data);
    }

    // TODO: Return an error that the function couldn't be found
    char *return_header = NULL;

    return return_header;
}

// TODO: Return the response to the route request
char *route_game(char *path, char *data) {
    char *route = NULL;
    if((route = strstr(path, "blackjack/")) != NULL) {
        return route_blackjack(route + strlen("blackjack/"), data);
    } 

    // TODO: Return an error header indicating that the game couldn't be found
    char *return_header = NULL;
    
    return return_header;
}