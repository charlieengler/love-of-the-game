#include <stdio.h>
#include <string.h>

#include "../../include/server/users/authenticate.h"

#include "../../include/server/utils/json.h"

char *find_user(struct json_value *db_json, char *req) {
    if (db_json->type != JSON_OBJECT) {
        // TODO: Fail
    }

    char *req_start = req;
    struct json_value *req_json = string_to_json_value(&req);
    req = req_start;

    if (req_json->type != JSON_OBJECT) {
        // TODO: Fail
    }

    char *user_id;
    // TODO: Error checking
    json_object_get_value((struct json_object *)(req_json->data), "userID", (void **)&user_id, JSON_STRING);

    struct json_object *user_entry_object;
    struct json_value *user_entry_json = json_object_get_value((struct json_object *)(db_json->data), user_id, (void **)&user_entry_object, JSON_OBJECT);

    struct json_object *res_object;
    struct json_value *res_json = create_object_json_value(&res_object);

    struct json_value *found_user_json;
    if (user_entry_json) {
        // TODO: Make sure sensitive fields are excluded from the return value
        found_user_json = user_entry_json;
    } else {
        found_user_json = create_false_json_value();
    }

    json_object_add_value(&res_object, "foundUser", found_user_json, NULL, found_user_json->type);

    char *res = json_value_to_string(res_json);

    return res;
}

char *login_user(struct json_value *db_json, char *req) {
    if (db_json->type != JSON_OBJECT) {
        // TODO: Fail
    }

    struct json_object *db_object = db_json->data;

    char *req_start = req;
    struct json_value *req_json = string_to_json_value(&req);
    req = req_start;

    if (req_json->type != JSON_OBJECT) {
        // TODO: Fail
    }

    char *user_id;
    // TODO: Error checking
    json_object_get_value((struct json_object *)(req_json->data), "userID", (void **)&user_id, JSON_STRING);

    char *password;
    // TODO: Error checking
    json_object_get_value((struct json_object *)(req_json->data), "password", (void **)&password, JSON_STRING);

    struct json_object *user_object;
    struct json_value *user_json = json_object_get_value(db_object, user_id, (void **)&user_object, JSON_OBJECT);
    if (!user_json) {
        // TODO: Handle user not found
    }

    char *user_password;
    // TODO: Error checking
    json_object_get_value(user_object, "password", (void **)&user_password, JSON_STRING);

    if (strcmp(user_password, password)) {
        // TODO: Handle password mismatch
        printf("Passwords did not match (%s != %s)\n", user_password, password);
    }

    // TODO: Error checking
    json_object_remove_value(&user_object, "logged_in");

    // TODO: Error checking
    json_object_add_value(&user_object, "logged_in", create_true_json_value(), NULL, JSON_TRUE);

    // TODO: Return value
    char *res = json_value_to_string(user_json);

    return res;
}

char *create_user(struct json_value *db_json, char *req) {
    if (db_json->type != JSON_OBJECT) {
        // TODO: Fail
    }

    struct json_object *db_object = db_json->data;

    char *req_start = req;
    struct json_value *req_json = string_to_json_value(&req);
    req = req_start;

    if (req_json->type != JSON_OBJECT) {
        // TODO: Fail
    }

    char *user_id;
    // TODO: Error checking
    json_object_get_value((struct json_object *)(req_json->data), "userID", (void **)&user_id, JSON_STRING);

    // TODO: Error checking
    struct json_value *password_json = json_object_get_value((struct json_object *)(req_json->data), "password", NULL, JSON_STRING);

    // TODO: Check to make double sure the user doesn't exist

    struct json_object *user_object;
    struct json_value *user_json = create_object_json_value(&user_object);

    // TODO: Storing password as plaintext lmao
    json_object_add_value(&user_object, "password", password_json, NULL, JSON_STRING);

    json_object_add_value(&user_object, "logged_in", create_false_json_value(), NULL, JSON_FALSE);

    json_object_add_value(&db_object, user_id, user_json, NULL, JSON_OBJECT);

    struct json_object *res_object;
    struct json_value *res_json = create_object_json_value(&res_object);

    json_object_add_value(&res_object, "createdUser", create_true_json_value(), NULL, JSON_TRUE);

    char *res = json_value_to_string(res_json);

    return res;
}
