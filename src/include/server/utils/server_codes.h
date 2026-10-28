#ifndef SERVER_CODES_H
#define SERVER_CODES_H

char *generate_http_status(int code);
int send_http_error(int error, int sock);

#endif // SERVER_CODES_H
