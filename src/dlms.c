#include "dlms.h"
#include <string.h>

size_t data_type_len(enum dlms_data_type type) {
    switch (type) {
        case BOOLEAN:
        case INTEGER:
        case UNSIGNED:
        case ENUM:
            return 1;
            break;

        case LONG:
        case LONG_UNSIGNED:
            return 2;
            break;

        case DOUBLE_LONG:
        case DOUBLE_LONG_UNSIGNED:
            return 4;
            break;

        case LONG64:
        case LONG64_UNSIGNED:
            return 8;
            break;

        // Types with non fixed sizes (and NULL)
        default:
            return 0;
            break;
    }
}

void* reverse_bytes(void *inp, size_t len)
{
    unsigned int i;
    unsigned char *in=(unsigned char *)inp,tmp;

    for(i=0;i<len/2;i++) {
        tmp=*(in+i);
        *(in+i)=*(in+len-i-1);
        *(in+len-i-1)=tmp;
    }

    return inp;
}

size_t dlms_parse_payload(struct dlms_object* obj, char* ptr) {
    union dlms_payload result;
    size_t size = 0;
    result.raw = 0;
    switch(obj->type) {
        case COMPACT_ARRAY:
        case STRUCT:
            result.raw = malloc(sizeof(struct dlms_object) * obj->size);
            for(int i = 0; i < obj->size; i++) {
                size += dlms_parse_object((struct dlms_object*)result.raw + i, ptr + size);
            }
            break;

        case ARRAY:
            result.raw = ptr;
            break;

        case BIT_STRING:
        case OCTET_STRING:
        case VISIBLE_STRING:
        case UTF8_STRING:
            size = obj->size;
            result.raw = malloc(size);
            memcpy(result.raw, ptr, size); 
            break;

        case BOOLEAN:
        case DOUBLE_LONG:
        case DOUBLE_LONG_UNSIGNED:
        case INTEGER:
        case LONG:
        case UNSIGNED:
        case LONG_UNSIGNED:
        case LONG64:
        case LONG64_UNSIGNED:
        case FLOAT32:
        case FLOAT64:
        case ENUM:
            size = obj->size;
            result.raw = malloc(size);
            memcpy(result.raw, ptr, size);
            result.raw = reverse_bytes(result.raw, size);
            break;


        case NULL_DATA:
            size = 0;
    }   

    obj->payload = result;
    return size;
}

/**
 * @brief Get the size of an object from pdu
 * 
 * This method returns type bytes and payload sizes separetely.
 * The type bytes is the bytes that secify the data type of the object.
 * If the object has a fixed length data type (such as INTEGER), there is 1 type byte.
 * Variable length types (such as ARRAY or BIT_STRING) need 2 type bytes.
 * 
 * For data types ARRAY and STRUCT, number of elements is returned instead of size in bytes.
 * 
 * @param len Output variable where the size of the object (Excluding the type bytes) is written.
 * @param type_bytes Output variable where the number of type bytes is written.
 * @param size The raw bytes to parse.
 */
void get_size_from_pdu(uint8_t* size, uint8_t* type_bytes, char* pdu) {
    enum dlms_data_type type = (enum dlms_data_type)pdu[0];
    size[0] = data_type_len(type);

    bool fixed_size = *size || !type;
    type_bytes[0] = 1;

    if(!fixed_size) {
        size[0] = pdu[1];
        type_bytes[0] = 2;
    }
}

size_t dlms_parse_object(struct dlms_object* dest, char* pdu) {
    uint8_t type_bytes = 0;

    dest->type = pdu[0];
    get_size_from_pdu(&dest->size, &type_bytes, pdu);
    size_t payload_size = dlms_parse_payload(dest, pdu + type_bytes);

    return payload_size + type_bytes;
}

size_t step_over(char* pdu) {
    enum dlms_data_type type = (enum dlms_data_type)pdu[0];
    size_t size = 0;
    uint8_t obj_size = 0;
    get_size_from_pdu(&obj_size, &size, pdu);
    if(type == ARRAY || type == STRUCT) {
        for(int i = 0; i < obj_size; i++) {
            size += step_over(pdu + size);
        }
    } else {
        size += obj_size;
    }
    return size;
}

size_t dlms_parse_object_in_array(struct dlms_object* dest, struct dlms_object array, uint8_t index) {
    int pos = 0;
    for(int i = 0; i < index; i++) {
        pos += step_over(array.payload.raw + pos);
    }
    return dlms_parse_object(dest, array.payload.raw + pos);
}

void dlms_free_object(struct dlms_object obj) {
    if(obj.type == ARRAY) {
        return;
    } 
    else if(obj.type == STRUCT) {
        for(int i = 0; i < obj.size; i++) {
            dlms_free_object(obj.payload.STRUCT[i]);
        }
    }
    if(obj.size > 0) {
        free(obj.payload.raw);
    }
}

#include <stdio.h>

struct dlms_frame dlms_parse_frame(char* buf) {
    struct dlms_frame frame;
    uint8_t read = 1;
    uint16_t frame_format = (buf[read] << 8) | (buf[read + 1] & 0x00ff);
    read++;
    frame.frame_length = frame_format & 0b11111111111;
    frame.dest_address = 0;
    frame.source_address = 0;
    uint8_t dest_addr_len = 1;
    for(; dest_addr_len < 5; dest_addr_len++) {
        frame.dest_address |= (buf[read + dest_addr_len] & 0x000000ff);
        if((frame.dest_address & 1) == 1)
            break;
        frame.dest_address = frame.dest_address << 8;
    }
    read += dest_addr_len;
    uint8_t source_addr_len = 1;
    for(; source_addr_len < 5; source_addr_len++) {
        frame.source_address |= (buf[read + source_addr_len] & 0x000000ff);
        if((frame.source_address & 1) == 1)
            break;
        frame.source_address = frame.source_address << 8;
    }
    read += source_addr_len;
    frame.control = buf[++read];
    frame.data_notification = dlms_parse_data_notification(buf + read + 3);
    return frame;
}

struct dlms_data_notification dlms_parse_data_notification(char* buf) {
    struct dlms_data_notification notif;
    uint8_t read = 3;
    notif.tag = buf[read++];
    notif.long_invoke_id_and_priority = (buf[read] << 24) | (buf[read+1] << 16) | (buf[read+2] << 8) | buf[read+3];
    read += 4;
    if(buf[read++]){
        notif.date_time = buf + read;
        read += 12;
    }
    dlms_parse_object(&notif.notification_body, buf + read);
    return notif;
}