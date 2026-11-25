#include <stdio.h>
#include <string.h>
#include <time.h>

#include "../../include/logging.h"

#include "../../include/server/users/authenticate.h"

#include "../../include/server/utils/json.h"

int find_user(struct json_value *db_json, char *req, char **send_buffer) {
    check_json_type(db_json->type, JSON_OBJECT, return 500);

    char *req_start = req;
    struct json_value *req_json = string_to_json_value(&req);
    req = req_start;

    check_json_type(req_json->type, JSON_OBJECT, return 500);

    char *user_id;
    if (!json_object_get_value((struct json_object *)(req_json->data), "userID", (void **)&user_id, JSON_STRING)) {
        printd("unable to find userID in req_object\n");

        return 500;
    }

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

    if (json_object_add_value(&res_object, "foundUser", found_user_json, NULL, found_user_json->type)) {
        printd("unable to add foundUser to res_object\n");

        return 500;
    }

    *send_buffer = json_value_to_string(res_json);

    if (!*send_buffer) {
        printd("send_buffer was null\n");

        return 500;
    }

    return 200;
}

int login_user(struct json_value *db_json, char *req, char **send_buffer) {
    check_json_type(db_json->type, JSON_OBJECT, return 500);

    struct json_object *db_object = db_json->data;

    char *req_start = req;
    struct json_value *req_json = string_to_json_value(&req);
    req = req_start;

    check_json_type(req_json->type, JSON_OBJECT, return 500);

    char *user_id;
    if (!json_object_get_value((struct json_object *)(req_json->data), "userID", (void **)&user_id, JSON_STRING)) {
        printd("unable to get userID from req_object\n");

        return 500;
    }

    char *password;
    if (!json_object_get_value((struct json_object *)(req_json->data), "password", (void **)&password, JSON_STRING)) {
        printd("unable to get password from req_object\n");

        return 500;
    }

    struct json_object *user_object;
    struct json_value *user_json = json_object_get_value(db_object, user_id, (void **)&user_object, JSON_OBJECT);
    if (!user_json) {
        // TODO: Handle user not found
    }

    char *user_password;
    if (!json_object_get_value(user_object, "password", (void **)&user_password, JSON_STRING)) {
        printd("unable to get password from user_object\n");

        return 500;
    }

    if (strcmp(user_password, password)) {
        // TODO: Handle password mismatch
        printf("Passwords did not match (%s != %s)\n", user_password, password);
    }

    if (json_object_remove_value(&user_object, "logged_in")) {
        printd("unable to remove logged_in value from user_object\n");

        return 500;
    }

    if (json_object_add_value(&user_object, "logged_in", create_true_json_value(), NULL, JSON_TRUE)) {
        printd("unable to add logged_in value to user_object\n");

        return 500;
    }

    if (json_object_remove_value(&user_object, "last_login")) {
        printd("unable to remove last_login from user_object\n");

        return 500;
    }

    struct json_object *last_login_object;
    struct json_value *last_login_json = create_object_json_value(&last_login_object);

    if (!last_login_json) {
        printd("create_object_json_value() returned null\n");

        return 500;
    }

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    struct json_value *year_json = create_number_json_value(NULL, t->tm_year + 1900, 0, 0, JSON_INTEGER);
    if (!year_json) {
        printd("create_number_json_value() returned null\n");

        return 500;
    }

    struct json_value *month_json = create_number_json_value(NULL, t->tm_mon + 1, 0, 0, JSON_INTEGER);
    if (!month_json) {
        printd("create_number_json_value() returned null\n");

        return 500;
    }

    struct json_value *day_json = create_number_json_value(NULL, t->tm_mday, 0, 0, JSON_INTEGER);
    if (!day_json) {
        printd("create_number_json_value() returned null\n");

        return 500;
    }

    struct json_value *hour_json = create_number_json_value(NULL, t->tm_hour, 0, 0, JSON_INTEGER);
    if (!hour_json) {
        printd("create_number_json_value() returned null\n");

        return 500;
    }

    struct json_value *minute_json = create_number_json_value(NULL, t->tm_min, 0, 0, JSON_INTEGER);
    if (!minute_json) {
        printd("create_number_json_value() returned null\n");

        return 500;
    }

    struct json_value *second_json = create_number_json_value(NULL, t->tm_sec, 0, 0, JSON_INTEGER);
    if (!second_json) {
        printd("create_number_json_value() returned null\n");

        return 500;
    }

    if (json_object_add_value(&last_login_object, "year", year_json, NULL, JSON_NUMBER)) {
        printd("unable to add year to last_login_object\n");

        return 500;
    }

    if (json_object_add_value(&last_login_object, "month", month_json, NULL, JSON_NUMBER)) {
        printd("unable to add month to last_login_object\n");

        return 500;
    }

    if (json_object_add_value(&last_login_object, "day", day_json, NULL, JSON_NUMBER)) {
        printd("unable to add day to last_login_object\n");

        return 500;
    }

    if (json_object_add_value(&last_login_object, "hour", hour_json, NULL, JSON_NUMBER)) {
        printd("unable to add hour to last_login_object\n");

        return 500;
    }

    if (json_object_add_value(&last_login_object, "minute", minute_json, NULL, JSON_NUMBER)) {
        printd("unable to add minute to last_login_object\n");

        return 500;
    }

    if (json_object_add_value(&last_login_object, "second", second_json, NULL, JSON_NUMBER)) {
        printd("unable to add second to last_login_object\n");

        return 500;
    }

    if (json_object_add_value(&user_object, "last_login", last_login_json, NULL, JSON_OBJECT)) {
        printd("unable to add last_login to user_object\n");

        return 500;
    }

    *send_buffer = json_value_to_string(user_json);

    if (!*send_buffer) {
        printd("send_buffer was null\n");

        return 500;
    }

    return 200;
}

// TODO: Some issue with create_user, corrupts db
int create_user(struct json_value *db_json, char *req, char **send_buffer) {
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
    if (!json_object_get_value((struct json_object *)(req_json->data), "userID", (void **)&user_id, JSON_STRING)) {
        printd("unable to get userID from req_json\n");

        return 500;
    }

    struct json_value *password_json = json_object_get_value((struct json_object *)(req_json->data), "password", NULL, JSON_STRING);
    if (!password_json) {
        printd("unable to get password from req_json\n");

        return 500;
    }

    // TODO: Check to make double sure the user doesn't exist

    struct json_object *user_object;
    struct json_value *user_json = create_object_json_value(&user_object);
    if (!user_json) {
        printd("create_object_json_value() returned null\n");

        return 500;
    }

    // TODO: Storing password as plaintext lmao
    if (json_object_add_value(&user_object, "password", password_json, NULL, JSON_STRING)) {
        printd("unable to add password to user_object\n");

        return 500;
    }

    if (json_object_add_value(&user_object, "logged_in", create_false_json_value(), NULL, JSON_FALSE)) {
        printd("unable to add logged_in to user_object\n");

        return 500;
    }

    if (json_object_add_value(&user_object, "balance", create_number_json_value(NULL, 1000, 0, 0, JSON_INTEGER), NULL, JSON_NUMBER)) {
        printd("unable to add balance to user_object\n");

        return 500;
    }

    if (json_object_add_value(&db_object, user_id, user_json, NULL, JSON_OBJECT)) {
        printd("unable to add %s to db_object\n", user_id);

        return 500;
    }

    struct json_object *res_object;
    struct json_value *res_json = create_object_json_value(&res_object);
    if (!res_json) {
        printd("create_object_json_value() returned null\n");

        return 500;
    }

    if (json_object_add_value(&res_object, "createdUser", create_true_json_value(), NULL, JSON_TRUE)) {
        printd("unable to add createdUser to res_object\n");

        return 500;
    }

    *send_buffer = json_value_to_string(res_json);

    if (!*send_buffer) {
        printd("send_buffer was null\n");

        return 500;
    }

    return 200;
}

int logout_user(struct json_value *db_json, char *req, char **send_buffer) {
    check_json_type(db_json->type, JSON_OBJECT, return 500);

    struct json_object *db_object = db_json->data;

    char *req_start = req;
    struct json_value *req_json = string_to_json_value(&req);
    req = req_start;

    if (!req_json) {
        printd("req_json was null\n");

        return 500;
    }

    check_json_type(req_json->type, JSON_OBJECT, return 500);

    char *user_id;
    if (!json_object_get_value((struct json_object *)(req_json->data), "userID", (void **)&user_id, JSON_STRING)) {
        printd("unable to get userID from req_object\n");
    }

    struct json_object *user_object;
    struct json_value *user_json = json_object_get_value(db_object, user_id, (void **)&user_object, JSON_OBJECT);
    if (!user_json) {
        // TODO: Handle user not found
    }

    if (json_object_remove_value(&user_object, "logged_in")) {
        printd("unable to remove logged_in from user_object\n");

        return 500;
    }

    if (json_object_add_value(&user_object, "logged_in", create_false_json_value(), NULL, JSON_FALSE)) {
        printd("unable to add logged_in to user_object\n");

        return 500;
    }

    struct json_object *res_object;
    struct json_value *res_json = create_object_json_value(&res_object);
    if (!res_json) {
        printd("create_object_json_value() returned null\n");

        return 500;
    }

    if (json_object_add_value(&res_object, "loggedOut", create_true_json_value(), NULL, JSON_TRUE)) {
        printd("unable to add loggedOut to res_object\n");

        return 500;
    }

    *send_buffer = json_value_to_string(res_json);

    if (!*send_buffer) {
        printd("send_buffer was null\n");

        return 500;
    }

    return 200;
}
