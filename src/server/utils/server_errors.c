#include "../../include/server/utils/server_errors.h"

int send_http_error(int error, int sock) {
    char *header = "HTTP/1.1 418 I'm a teapot\r\n\r\n";

    switch(error) {
        case 400:
            header = "HTTP/1.1 400 Bad Request\r\n\r\n";
            break;
        case 401:
            header = "HTTP/1.1 401 Unauthorized\r\n\r\n";
            break;
        case 404:
            header = "HTTP/1.1 404 Not Found\r\n\r\n";
            break;
        case 405:
            header = "HTTP/1.1 405 Method Not Allowed\r\n\r\n";
            break;
        case 500:
            header = "HTTP/1.1 500 Internal Server Error\r\n\r\n";
            break;
        case 505:
            header = "HTTP/1.1 505 HTTP Version Not Supported\r\n\r\n";
            break;
    }

    int send_size = send(sock, header, strlen(header), 0);
    if(send_size == -1) {
        printf("[./server/server.c | send_http_error()] send() error: size of header was negative\n");
        return 1;
    } else if(send_size != strlen(header)) {
        printf("[./server/server.c | send_http_error()] send() error: size of header doesn't match size of data sent\n");
        return 1;
    }

    close(sock);

    return 0;
}