#include <stdio.h>

#include "../../../include/server/games/blackjack/blackjack.h"
#include "../../../include/server/utils/json_handler.h"

char *blackjack_place_bet(char *data) {
    // TODO: Actually store this bet with the data (use a json object, good luck with the database kiddo)
    printf("Placed Bet: %s\n", data);
    
    struct json_object *return_object = json_initialize_object();

    // TODO: Error handling
    json_add_entry(return_object, "success", "Bet Placed");

    // TODO: This needs to be freed at some point
    char *return_string = json_to_string(return_object);

    return return_string;
}