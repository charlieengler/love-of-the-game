#include <stdlib.h>
#include <string.h>

#include "../../include/server/database/database.h"
#include "../../include/server/games/games.h"

#include "../../include/server/games/blackjack/blackjack.h"

#include "./blackjack/internal.h"

int route_blackjack(char *function, char *data, char **send_buffer) {
    struct json_value *blackjack_db = db_initialize("blackjack");

    int code = 404;

    if (strstr(function, "join-table") != NULL) {
        code = blackjack_join_table(blackjack_db, data, send_buffer);
    } else if (strstr(function, "check-status") != NULL) {
        code = blackjack_status_update(blackjack_db, data, send_buffer);
    } else if (strstr(function, "bet") != NULL) {
        code = blackjack_place_bet(blackjack_db, data, send_buffer);
    } else if (strstr(function, "progress-hand") != NULL) {
        code = blackjack_progress_hand(blackjack_db, data, USER_NO_ACTION, send_buffer);
    } else if (strstr(function, "hit") != NULL) {
        code = blackjack_progress_hand(blackjack_db, data, USER_HIT, send_buffer);
    } else if (strstr(function, "stand") != NULL) {
        code = blackjack_progress_hand(blackjack_db, data, USER_STAND, send_buffer);
    }

    // TODO: Maybe check for an error code before saving the db

    db_save(blackjack_db, "blackjack");

    // TODO: Return an error that the function couldn't be found if the header is still NULL at this point
    if (code >= 400) {
        // TODO: Better return header error
        const char *header_const = "{\"error\":\"Make me more descriptive\"}";
        char *return_header = (char *)calloc(strlen(header_const) + 1, sizeof(char));
        strcpy(return_header, header_const);

        *send_buffer = return_header;
    }

    return code;
}

// TODO: Return the response to the route request
int route_game(char *path, char *data, char **send_buffer) {
    char *route = NULL;
    if ((route = strstr(path, "blackjack/")) != NULL) {
        return route_blackjack(route + strlen("blackjack/"), data, send_buffer);
    }

    const char *header_const = "{\"error\":\"Unknown game route\"}";
    char *return_header = (char *)calloc(strlen(header_const) + 1, sizeof(char));
    strcpy(return_header, header_const);

    *send_buffer = return_header;

    return 404;
}
