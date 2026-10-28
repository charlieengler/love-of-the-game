#include <stdlib.h>
#include <string.h>

#include "../../include/server/users/authenticate.h"
#include "../../include/server/users/users.h"

#include "../../include/server/database/database.h"

int route_auth(char *function, char *data, char **send_buffer) {
    struct json_value *users_db = db_initialize("users");

    int code = 404;

    if (strstr(function, "login") != NULL) {
        code = login_user(users_db, data, send_buffer);
    } else if (strstr(function, "find") != NULL) {
        code = find_user(users_db, data, send_buffer);
    } else if (strstr(function, "signup") != NULL) {
        code = create_user(users_db, data, send_buffer);
    } else if (strstr(function, "logout") != NULL) {
        code = logout_user(users_db, data, send_buffer);
    }

    // TODO: Maybe only save the db if no error code
    db_save(users_db, "users");

    // TODO: Return an error that the function couldn't be found if the header is still NULL at this point
    if (code >= 400) {
        // TODO: Better return header error
        const char *header_const = "{\"error\":\"Make me more descriptive\"}";
        char *return_header = (char *)calloc(strlen(header_const) + 1, sizeof(char));
        strcpy(return_header, header_const);

        *send_buffer = return_header;
    }

    return code;
}

int route_users(char *path, char *data, char **send_buffer) {
    char *route = NULL;
    if ((route = strstr(path, "auth/")) != NULL) {
        return route_auth(route + strlen("auth/"), data, send_buffer);
    }

    const char *header_const = "{\"error\":\"Unknown users route\"}";
    char *return_header = (char *)calloc(strlen(header_const) + 1, sizeof(char));
    strcpy(return_header, header_const);

    *send_buffer = return_header;

    return 404;
}
