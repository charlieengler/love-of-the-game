#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../include/server/games/blackjack/blackjack.h"

char *blackjack_place_bet(char *data) {
    // TODO: Actually store this bet with the data
    printf("Placed Bet: %s\n", data);
    
    char *tmp_msg = "{\"success\":\"Bet Placed\"}";
    char *return_header = (char*)malloc((strlen(tmp_msg) + 1) * sizeof(char));
    strcpy(return_header, tmp_msg);

    return return_header;
}