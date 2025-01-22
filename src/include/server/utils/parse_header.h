struct header
{
    char *method;
    char *path;
    float version;
    int content_length;
};

struct header parse_header(char*);
void print_header(struct header);
void delete_header(struct header);