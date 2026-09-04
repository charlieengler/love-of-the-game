#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../include/router.h"

#include "../include/server/games/games.h"

#include "../include/server/utils/server_errors.h"

static int split_string(char *input, char delimiter) {
    char *current_char = input;
    int num_chars = 0;
    while (*current_char) {
        if (*current_char == delimiter || *current_char == 0)
            return num_chars;

        num_chars++;
        current_char++;
    }

    return -1;
}

int route_get(char *trimmed_path, char *full_path, int new_fd, char **send_buffer, char **content_type) {
    if (strcmp(trimmed_path, "/") == 0 || strcmp(trimmed_path, "/index.html") == 0) {
        full_path = HTML_BASE_PATH INDEX_FILE;
    }

    if (access(full_path, F_OK) != 0) {
        send_http_error(404, new_fd);

        printf("[./server/server.c | run_server()] File %s doesn't exist on server\n", full_path);
        return 404;
    }

    long send_length = 0;
    FILE *send_file = fopen(full_path, "rb");

    if (send_file) {
        fseek(send_file, 0, SEEK_END);

        send_length = ftell(send_file);

        fseek(send_file, 0, SEEK_SET);

        // TODO: Malloc error checking
        *send_buffer = (char *)malloc((send_length + strlen("\r\n\r\n") + 1) * sizeof(char));
        fread(*send_buffer, sizeof(char), send_length, send_file);

        (*send_buffer)[send_length] = '\0';

        fclose(send_file);

        strcat(*send_buffer, "\r\n\r\n\0");
    }
    // TODO: Error here if the send file isn't opened

    const char *text_str = "Content-Type: text/";
    int type_offset = split_string(trimmed_path, '.') + 1;
    int type_length = strlen(&trimmed_path[type_offset]);
    const char *end_str = "; charset=UTF-8\r\n";

    *content_type = (char *)malloc(strlen(text_str) + type_length + strlen(end_str) + 1);
    strcpy(*content_type, text_str);
    strcat(*content_type, &trimmed_path[type_offset]);
    strcat(*content_type, end_str);
    (*content_type)[strlen(text_str) + type_length + strlen(end_str)] = '\0';

    return 0;
}

int route_post(char *path, char *data, char **send_buffer) {
    char *route = NULL;
    if ((route = strstr(path, "routes/games/")) != NULL) {
        *send_buffer = route_game(route + strlen("routes/games/"), data);

        return 0;
    }

    // TODO: Replace this with a better error message
    char *unknown_route_header = "{\"error\":\"Unknown Route\"}";
    char *return_header = (char *)calloc((strlen(unknown_route_header) + 1), sizeof(char));
    strcpy(return_header, unknown_route_header);

    *send_buffer = return_header;

    return 0;
}
