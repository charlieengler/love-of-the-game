#define HTML_BASE_PATH "./src/server/html"
#define INDEX_FILE "/index.html"

int route_get(char *trimmed_path, char *full_path, int new_fd, char **send_buffer, char **content_type);
int route_post(char *path, char *data, char **send_buffer);
