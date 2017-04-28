#include "serializer.h"

Serializer* serializer_init(int mpool_size) {
    Serializer* se = malloc(sizeof(Serializer));
    bzero(se, sizeof(Serializer));
    se->root = json_object();
    return se;
}

void serializer_free(Serializer* se) {
    if (se == NULL) return;
    json_decref(se->root);
    free(se);
}

char* serializer_to_string(Serializer* se) {
    return json_dumps(se->root, 0);
}

int deserialize_message(Serializer* se, const char* buffer, int size) {
    json_error_t error;

    json_object_clear(se->root);
    se->root = json_loads(buffer, 0, &error);
    
    if (!se->root) {
        return -1;
    }
    return 0;
}

int serialize_requestvote(Serializer* se, int myid, const msg_requestvote_t* msg) {
    json_object_clear(se->root);
    json_object_set_new(se->root, "myid", json_integer(myid));
    json_object_set_new(se->root, "message_type", json_string("request_vote"));
    json_object_set_new(se->root, "term", json_integer(msg->term));
    json_object_set_new(se->root, "candidate_id", json_integer(msg->candidate_id));
    json_object_set_new(se->root, "last_log_idx", json_integer(msg->last_log_idx));
    json_object_set_new(se->root, "last_log_term", json_integer(msg->last_log_term));
    return 0;
}

int deserialize_requestvote(Serializer* se, msg_requestvote_t* msg) {
    msg->term = json_integer_value(json_object_get(se->root, "term"));
    msg->candidate_id = json_integer_value(json_object_get(se->root, "candidate_id"));
    msg->last_log_idx = json_integer_value(json_object_get(se->root, "last_log_idx"));
    msg->last_log_term = json_integer_value(json_object_get(se->root, "last_log_term"));
    return 0;
}

int serialize_requestvote_response(Serializer* se, int myid, const msg_requestvote_response_t* msg) {
    json_object_clear(se->root);
    json_object_set_new(se->root, "myid", json_integer(myid));
    json_object_set_new(se->root, "message_type", json_string("request_vote_response"));
    json_object_set_new(se->root, "term", json_integer(msg->term));
    json_object_set_new(se->root, "vote_granted", json_integer(msg->vote_granted));
    return 0;
}

int deserialize_requestvote_response(Serializer* se, msg_requestvote_response_t* msg) {
    json_object_clear(se->root);
    msg->term = json_integer_value(json_object_get(se->root, "term"));
    msg->vote_granted = json_integer_value(json_object_get(se->root, "vote_granted"));
    return 0;
}

