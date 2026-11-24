#include "internal.h"

#include "../../../include/logging.h"

#include "../../../include/server/utils/json.h"

char *generate_table_res(struct json_value *table_json) {
    if (!table_json) {
        printd("table_json is null\n");

        return NULL;
    }

    check_json_type(table_json->type, JSON_OBJECT, return NULL);

    struct json_object *table_object = (struct json_object *)(table_json->data);

    int hand_progress;

    struct json_number *hand_progress_num;
    struct json_value *hand_progress_json = json_object_get_value(table_object, "hand-progress", (void **)&hand_progress_num, JSON_NUMBER);
    if (!hand_progress_json) {
        hand_progress = -1;
    } else {
        hand_progress = (int)(hand_progress_num->integer);
    }

    struct json_array *users_array;
    struct json_value *users_json = json_object_get_value(table_object, "users", (void **)&users_array, JSON_ARRAY);
    if (!users_json) {
        printd("unable to get users array from table_object\n");

        return NULL;
    }

    int num_users = users_array->length;

    struct json_object *dealer_object;
    struct json_value *dealer_json = json_object_get_value(table_object, "dealer", (void **)&dealer_object, JSON_OBJECT);
    if (!dealer_json) {
        printd("unable to get dealer from table_object\n");

        return NULL;
    }

    struct json_array *dealer_hand_arr;
    struct json_value *dealer_hand_json = json_object_get_value(dealer_object, "hand", (void **)&dealer_hand_arr, JSON_ARRAY);
    if (!dealer_hand_json) {
        dealer_hand_json = create_array_json_value(NULL);

        if (!dealer_hand_json) {
            printd("unable to create dealer_hand_json\n");

            return NULL;
        }

        if (!json_object_add_value(&dealer_object, "hand", dealer_hand_json, (void **)&dealer_hand_arr, JSON_ARRAY)) {
            printd("unable to add dealer_hand_json to dealer_object\n");

            return NULL;
        }
    }

    struct json_value *new_hand_json = json_object_get_value(table_object, "new-hand", NULL, JSON_TRUE);
    if (!new_hand_json) {
        printd("unable to get new_hand_json from table_object\n");
    }

    struct json_value *res_dealer_hand_json;
    struct json_value *active_user_json;
    if (hand_progress >= 0 && hand_progress < num_users) {
        active_user_json = users_array->values[hand_progress];
    } else {
        active_user_json = dealer_json;

        res_dealer_hand_json = dealer_hand_json;
    }

    if (hand_progress < num_users) {
        struct json_array *res_dealer_hand_arr;

        res_dealer_hand_json = create_array_json_value(&res_dealer_hand_arr);
        if (!res_dealer_hand_json) {
            printd("unable to create res_dealer_hand_json\n");

            return NULL;
        }

        if (dealer_hand_arr->length > 1) {
            if (json_array_add_value(&res_dealer_hand_arr, dealer_hand_arr->values[1])) {
                printd("unable to add dealer card to res_dealer_hand_arr\n");

                return NULL;
            }
        }
    }

    struct json_object *res_object;
    struct json_value *res_json = create_object_json_value(&res_object);
    if (!res_json) {
        printd("unable to create res_json\n");

        return NULL;
    }

    if (hand_progress >= num_users) {
        struct json_value *hand_ended_json = create_true_json_value();
        if (!hand_ended_json) {
            printd("unable to create hand_ended_json\n");

            return NULL;
        }

        if (json_object_add_value(&res_object, "handEnded", hand_ended_json, NULL, JSON_TRUE)) {
            printd("unable to add true hand_ended_json to res_object\n");

            return NULL;
        }
    } else {
        struct json_value *hand_ended_json = create_false_json_value();
        if (!hand_ended_json) {
            printd("unable to create hand_ended_json\n");

            return NULL;
        }

        if (json_object_add_value(&res_object, "handEnded", hand_ended_json, NULL, JSON_FALSE)) {
            printd("unable to add false hand_ended_json to res_object\n");

            return NULL;
        }
    }

    if (new_hand_json) {
        if (json_object_add_value(&res_object, "newHand", new_hand_json, NULL, JSON_TRUE)) {
            printd("unable to add new_hand_json to res_object\n");

            return NULL;
        }
    }

    if (json_object_add_value(&res_object, "users", users_json, NULL, JSON_ARRAY)) {
        printd("unable to add users_json to res_object\n");

        return NULL;
    }

    if (json_object_add_value(&res_object, "dealerCards", res_dealer_hand_json, NULL, JSON_ARRAY)) {
        printd("unable to add res_dealer_hand_json to res_object\n");

        return NULL;
    }

    if (json_object_add_value(&res_object, "activeUser", active_user_json, NULL, JSON_OBJECT)) {
        printd("unable to add active_user_json to res_object\n");

        return NULL;
    }

    char *res = json_value_to_string(res_json);
    if (!res) {
        printd("unable to convert res_json to string\n");

        return NULL;
    }

    return res;
}
