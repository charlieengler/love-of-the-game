#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../../include/server/utils/server_codes.h"

char *generate_http_status(int code) {
    char *header = "HTTP/1.1 418 I'm a teapot\r\n";

    switch (code) {
    case 200:
        header = "HTTP/1.1 200 OK\r\n";
        break;
    case 201:
        header = "HTTP/1.1 201 Created\r\n";
        break;
    case 202:
        header = "HTTP/1.1 202 Accepted\r\n";
        break;
    case 203:
        header = "HTTP/1.1 203 Non-Authoritative Information\r\n";
        break;
    case 204:
        header = "HTTP/1.1 204 No Content\r\n";
        break;
    case 205:
        header = "HTTP/1.1 205 Reset Content\r\n";
        break;
    case 206:
        header = "HTTP/1.1 206 Partial Content\r\n";
        break;
    case 207:
        header = "HTTP/1.1 207 Multi Status\r\n";
        break;
    case 208:
        header = "HTTP/1.1 208 Already Reported\r\n";
        break;
    case 226:
        header = "HTTP/1.1 226 IM Used\r\n";
        break;
    case 400:
        header = "HTTP/1.1 400 Bad Request\r\n";
        break;
    case 401:
        header = "HTTP/1.1 401 Unauthorized\r\n";
        break;
    case 404:
        header = "HTTP/1.1 404 Not Found\r\n";
        break;
    case 405:
        header = "HTTP/1.1 405 Method Not Allowed\r\n";
        break;
    case 500:
        header = "HTTP/1.1 500 Internal Server Error\r\n";
        break;
    case 505:
        header = "HTTP/1.1 505 HTTP Version Not Supported\r\n";
        break;
    }

    return header;
}

int send_http_error(int error, int sock) {
    char *header = generate_http_status(error);

    int send_len = strlen(header) + 3;

    char *tmp = (char *)malloc(send_len * sizeof(char));
    strcpy(tmp, header);
    strcat(tmp, "\r\n");
    tmp[send_len - 1] = '\0';

    int send_size = send(sock, tmp, send_len, 0);
    if (send_size == -1) {
        printf("[./server/utils/server_codes.c | send_http_error()] send() error: size of header was negative\n");
        return 1;
    } else if ((size_t)send_size != send_len) {
        printf("[./server/utils/server_codes.c | send_http_error()] send() error: size of header (%ld) doesn't match size of data sent (%ld)\n", (size_t)send_size, strlen(header));
        return 1;
    }

    close(sock);
    free(tmp);

    return 0;
}
