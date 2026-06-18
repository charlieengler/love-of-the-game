#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../include/router.h"

#include "../include/server/games/games.h"

#include "../include/server/utils/server_errors.h"

char *route_get(char *trimmed_path, char *full_path, int new_fd) {
    if (strcmp(trimmed_path, "/") == 0 || strcmp(trimmed_path, "/index.html") == 0) {
        full_path = HTML_BASE_PATH INDEX_FILE;
    }

    if (access(full_path, F_OK) != 0) {
        send_http_error(404, new_fd);

        printf("[./server/server.c | run_server()] File %s doesn't exist on server\n", full_path);
        return NULL;
    }

    char *send_buffer = NULL;
    long send_length;
    FILE *send_file = fopen(full_path, "rb");

    if (send_file) {
        fseek(send_file, 0, SEEK_END);
        send_length = ftell(send_file);
        fseek(send_file, 0, SEEK_SET);
        send_buffer = (char *)malloc((send_length + 1) * sizeof(char));
        if (send_buffer)
            fread(send_buffer, 1, send_length, send_file);

        fclose(send_file);

        send_buffer[send_length] = 0;
    }
    // TODO: Error here if the send file isn't opened

    return send_buffer;
}

char *route_post(char *path, char *data) {
    char *route = NULL;
    if ((route = strstr(path, "routes/games/")) != NULL) {
        char *res = route_game(route + strlen("routes/games/"), data);

        return res;
    }

    // TODO: Replace this with a better error message
    char *unknown_route_header = "{\"error\":\"Unknown Route\"}";
    char *return_header = (char *)calloc((strlen(unknown_route_header) + 1), sizeof(char));
    strcpy(return_header, unknown_route_header);

    return return_header;
}
