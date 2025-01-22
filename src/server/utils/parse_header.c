#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/server/utils/parse_header.h"

/*
    GET /games/blackjack.html HTTP/1.1
    Host: localhost:3000
    User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:134.0) Gecko/20100101 Firefox/134.0
    Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*\/*;q=0.8
    Accept-Language: en-US,en;q=0.5
    Accept-Encoding: gzip, deflate, br, zstd
    Connection: keep-alive
    Upgrade-Insecure-Requests: 1
    Sec-Fetch-Dest: document
    Sec-Fetch-Mode: navigate
    Sec-Fetch-Site: cross-site
    Priority: u=0
*/

/*
    POST /routes/games/blackjack/bet HTTP/1.1
    Host: localhost:3000
    User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:134.0) Gecko/20100101 Firefox/134.0
    Accept: *\/*
    Accept-Language: en-US,en;q=0.5
    Accept-Encoding: gzip, deflate, br, zstd
    Referer: http://localhost:3000/games/blackjack.html
    Content-type: application/json
    Content-Length: 12
    Origin: http://localhost:3000
    Connection: keep-alive
    Sec-Fetch-Dest: empty
    Sec-Fetch-Mode: cors
    Sec-Fetch-Site: same-origin
    Priority: u=0
*/

int split_string(char *input, char delimiter)
{
    char *current_char = input;
    int num_chars = 0;
    while(*current_char)
    {
        if(*current_char == delimiter || *current_char == 0)
            return num_chars;

        num_chars++;
        current_char++;
    }

    return -1;
}

struct header parse_header(char *raw_header)
{
    struct header return_header;
    return_header.method = "UNDEFINED";
    return_header.path = NULL;
    return_header.version = -1.0f;
    return_header.content_length = -1;

    char *copied_header = (char*)malloc(strlen(raw_header) * sizeof(char));
    strcpy(copied_header, raw_header);

    char **lines = (char**)malloc(sizeof(char*));

    char *current_char = copied_header;
    char *current_line = copied_header;
    int num_lines = 0;
    while(*current_char)
    {
        if(*current_char == '\n')
        {
            *current_char = 0;

            char **new_lines = (char**)malloc(((++num_lines)+1) * sizeof(char*));
            new_lines = lines;
            new_lines[num_lines-1] = current_line;
            lines = new_lines;

            current_line = current_char + 1;
        }

        current_char++;
    }

    char **strings = (char**)malloc(sizeof(char*));
    int num_strings = 0;
    for(int i = 0; i < num_lines; i++)
    {
        char *line = lines[i];
        int offset = 0;
        while((offset = split_string(line, ' ')) >= 0)
        {
            num_strings++;

            line[offset] = 0;

            char **new_strings = (char**)malloc(num_strings * sizeof(char*));
            new_strings = strings;
            new_strings[num_strings-1] = line;
            strings = new_strings;

            line += offset + 1;
        }

        num_strings++;

        char **new_strings = (char**)malloc(num_strings * sizeof(char*));
        new_strings = strings;
        new_strings[num_strings-1] = line;
        strings = new_strings;
    }

    return_header.method = (char*)malloc(strlen(strings[0]) * sizeof(char));
    strcpy(return_header.method, strings[0]);

    return_header.path = (char*)malloc(strlen(strings[1]) * sizeof(char));
    strcpy(return_header.path, strings[1]);

    char *version_string = strings[2] + split_string(strings[2], '/') + 1;
    return_header.version = atof(version_string);

    if(!strcmp(return_header.method, "POST"))
        return_header.content_length = atoi(strings[29]);

    free(lines);
    free(strings);
    free(copied_header);

    return return_header;
}

void print_header(struct header input)
{
    printf("---- HTTP HEADER ----\n");

    printf("    Method: %s\n", input.method);
    printf("    Path: %s\n", input.path);
    printf("    HTTP Version: %f\n", input.version);
    printf("    Content Length: %d\n", input.content_length);
}

void delete_header(struct header input)
{
    free(input.method);
    free(input.path);
}