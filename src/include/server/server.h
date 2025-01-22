#define RCVBUFSIZE 1024
#define HTTP_VERSION "1.1"
#define HTML_BASE_PATH "./src/server/html/"
#define INDEX_FILE "index.html"
#define INPUT_PORT "3000"

#define NUM_ALLOWED_METHODS 2
#define NUM_DISALLOWED_METHODS 3

int initialize_server();
int run_server(int sockfd);