#ifndef DLMS_H
#define DLMS_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define CHOICE 0

struct dlms_object;

enum dlms_data_type {
    NULL_DATA = 0,
    ARRAY = 1,
    STRUCT = 2,
    BOOLEAN = 3,
    BIT_STRING = 4,
    DOUBLE_LONG = 5,
    DOUBLE_LONG_UNSIGNED = 6,
    OCTET_STRING = 9,
    VISIBLE_STRING = 10,
    UTF8_STRING = 12,
    INTEGER = 15,
    LONG = 16,
    UNSIGNED = 17,
    LONG_UNSIGNED = 18,
    COMPACT_ARRAY = 19,
    LONG64 = 20,
    LONG64_UNSIGNED = 21,
    ENUM = 22,
    FLOAT32 = 23,
    FLOAT64 = 24
};

union dlms_payload {
    void* raw;
    struct dlms_object* ARRAY;
    struct dlms_object* STRUCT;
    char* BIT_STRING;
    uint8_t* OCTET_STRING;
    char* VISIBLE_STRING;
    char* UTF8_STRING;
    bool* BOOLEAN;
    int32_t* DOUBLE_LONG;
    uint32_t* DOUBLE_LONG_UNSIGNED;
    int8_t* INTEGER;
    uint8_t* UNSIGNED;
    int16_t* LONG;
    uint16_t* LONG_UNSIGNED;
    int64_t* LONG64;
    uint64_t* LONG64_UNSIGNED;
};

struct dlms_object {
    enum dlms_data_type type;
    uint8_t size;
    union dlms_payload payload;
};

struct dlms_data_notification {
    uint8_t tag;
    uint32_t long_invoke_id_and_priority;
    char* date_time;
    struct dlms_object notification_body;
};

struct dlms_frame {
    uint16_t frame_length;
    uint32_t dest_address;
    uint32_t source_address;
    uint8_t control;
    struct dlms_data_notification data_notification;
};

struct dlms_frame dlms_parse_frame(char* buf);
struct dlms_data_notification dlms_parse_data_notification(char* buf);
size_t dlms_parse_object(struct dlms_object* dest, char* buf);


void dlms_free_object(struct dlms_object);

#ifdef __cplusplus
}
#endif
#endif