#include <stdlib.h>
#include <string.h>

#include "../../include/server/users/authenticate.h"
#include "../../include/server/users/users.h"

#include "../../include/server/database/database.h"

char *route_auth(char *function, char *data) {
    struct json_value *users_db = db_initialize("users");

    char *return_header = NULL;

    if (strstr(function, "login") != NULL) {
        return_header = login_user(users_db, data);
    } else if (strstr(function, "find") != NULL) {
        return_header = find_user(users_db, data);
    } else if (strstr(function, "signup") != NULL) {
        return_header = create_user(users_db, data);
    } else if (strstr(function, "logout") != NULL) {
        return_header = logout_user(users_db, data);
    }

    db_save(users_db, "users");

    // TODO: Return an error that the function couldn't be found if the header is still NULL at this point
    if (return_header == NULL) {
        // TODO: Better return header error
        const char *header_const = "{\"error\":\"Make me more descriptive\"}";
        return_header = (char *)calloc(strlen(header_const) + 1, sizeof(char));
        strcpy(return_header, header_const);
    }

    return return_header;
}

char *route_users(char *path, char *data) {
    char *route = NULL;
    if ((route = strstr(path, "auth/")) != NULL) {
        char *res = route_auth(route + strlen("auth/"), data);

        return res;
    }

    const char *header_const = "{\"error\":\"Unknown users route\"}";
    char *return_header = (char *)calloc(strlen(header_const) + 1, sizeof(char));
    strcpy(return_header, header_const);

    return return_header;
}
