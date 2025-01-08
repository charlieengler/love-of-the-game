#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define RCVBUFSIZE 1024
#define HTTP_VERSION "1.1"
#define INDEX_FILE "./src/server/html/index.html"
#define INPUT_PORT "3000"

#define NUM_ALLOWED_METHODS 1
const char *allowed_methods[NUM_ALLOWED_METHODS] = { "GET" };

#define NUM_DISALLOWED_METHODS 4
const char *disallowed_methods[NUM_DISALLOWED_METHODS] = { "POST", "HEAD", "PUT", "DELETE" };

int initialize_server();
int run_server(int sockfd);