#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../include/server/server.h"
#include "../include/server/utils/parse_header.h"
#include "../include/server/utils/server_errors.h"

#include "../include/router.h"

int errno;

int initialize_server() {
    int status;
    struct addrinfo hints;
    struct addrinfo *serv_info;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((status = getaddrinfo(0, INPUT_PORT, &hints, &serv_info)) != 0) {
        fprintf(stderr, "[./server/server.c | initialize_server()] getaddrinfo() error: %s\n", gai_strerror(status));
        exit(1);
    }

    int sockfd;
    if ((sockfd = socket(serv_info->ai_family, serv_info->ai_socktype, serv_info->ai_protocol)) == -1) {
        printf("[./server/server.c | initialize_server()] socket() error: %s\n", strerror(errno));
        exit(1);
    }

    int yes = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        printf("[./server/server.c | initialize_server()] setsockopt() error: %s\n", strerror(errno));
        exit(1);
    }

    int bind_status;
    if ((bind_status = bind(sockfd, serv_info->ai_addr, serv_info->ai_addrlen)) == -1) {
        printf("[./server/server.c | initialize_server()] bind() error: %s\n", strerror(errno));
        exit(1);
    }

    int listen_status;
    if ((listen_status = listen(sockfd, 5)) == -1) {
        printf("[./server/server.c | initialize_server()] listen() error: %s\n", strerror(errno));
        exit(1);
    }

    run_server(sockfd);

    close(sockfd);

    return 0;
}

// TODO: There is an error when reloading the page then sending a blackjack bet request

int run_server(int sockfd) {
    struct sockaddr_storage incoming_addr;
    socklen_t addr_size = sizeof(incoming_addr);
    int new_fd;
    while ((new_fd = accept(sockfd, (struct sockaddr *)&incoming_addr, &addr_size)) >= 0) {
        char *tmp_buf = (char *)malloc(RCVBUFSIZE * sizeof(char));
        char *buf = (char *)malloc((RCVBUFSIZE + 1) * sizeof(char));

        printf("Receiving %ld bytes from client.\n", recv(new_fd, buf, RCVBUFSIZE, MSG_PEEK | MSG_TRUNC));

        int read_size = 0;
        int total_read_size = 0;
        int num_packets = 1;
        do {
            read_size = recv(new_fd, tmp_buf, RCVBUFSIZE, 0);

            buf = (char *)malloc((RCVBUFSIZE * num_packets + 1) * sizeof(char));
            strcpy(buf, tmp_buf);

            num_packets++;
            total_read_size += read_size;
        } while (read_size == RCVBUFSIZE);

        if (total_read_size <= 0) {
            send_http_error(400, new_fd);

            if (total_read_size == 0)
                printf("[./server/server.c | run_server()] recv() error: no header received\n");
            else
                printf("[./server/server.c | run_server()] recv() error: %s\n", strerror(errno));

            continue;
        }

        buf[total_read_size] = '\0';

        struct header recv_header = parse_header(buf);

        if (recv_header.version != HTTP_VERSION) {
            send_http_error(505, new_fd);

            printf("[./server/server.c | run_server()] Incompatible HTTP Version in incoming header %f\n", recv_header.version);
            continue;
        }

        char *find_path = (char *)malloc((strlen(HTML_BASE_PATH) + strlen(recv_header.path) + 1) * sizeof(char));
        strcpy(find_path, HTML_BASE_PATH);
        strcat(find_path, recv_header.path);
        find_path[strlen(HTML_BASE_PATH) + strlen(recv_header.path)] = 0;

        char *send_buffer = NULL;

        char *content_type = NULL;
        if (!strcmp(recv_header.method, "GET")) {
            route_get(recv_header.path, find_path, new_fd, &send_buffer, &content_type);
        } else if (!strcmp(recv_header.method, "POST")) {
            route_post(find_path, recv_header.content, &send_buffer);
        } else {
            send_http_error(400, new_fd);

            printf("[./server/server.c | run_server()] Undefined HTTP Method in incoming header\n");
            continue;
        }

        if (send_buffer == NULL)
            continue;

        // TODO: Better response header
        char *response_type = "HTTP/1.1 200 OK\r\n";

        int content_length = strlen(send_buffer);
        char *response_length = (char *)malloc(241 * sizeof(int) / 100 + 20);

        sprintf(response_length, "Content-Length: %d\r\n\r\n", content_length);

        int response_buffer_length = strlen(response_type) + strlen(response_length) + strlen(send_buffer) + 1;

        if (content_type) {
            response_buffer_length += strlen(content_type);
        }

        char *response_buffer = (char *)malloc(response_buffer_length * sizeof(char));

        strcpy(response_buffer, response_type);
        if (content_type) {
            strcat(response_buffer, content_type);
        }
        strcat(response_buffer, response_length);
        strcat(response_buffer, send_buffer);

        response_buffer[response_buffer_length - 1] = '\0';

        if (content_type) {
            free(content_type);
        }
        free(response_length);
        // TODO: This causes an invalid next size for some reason
        // free(send_buffer);

        long total_send_size = 0;
        while ((size_t)total_send_size < strlen(response_buffer)) {
            int send_size = send(new_fd, &response_buffer[total_send_size], strlen(response_buffer) - total_send_size, 0);
            if (send_size == -1) {
                send_http_error(500, new_fd);

                printf("send() error\n");
            }

            total_send_size += send_size;
        }

        printf("Sent %ld bytes\n", total_send_size);

        delete_header(&recv_header);
        free(buf);
        free(find_path);
        free(response_buffer);

        close(new_fd);
    }

    return 0;
}
