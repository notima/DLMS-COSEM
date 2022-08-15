#include "dlms.h"

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
    switch(obj->type) {
        case ARRAY:
        case STRUCT:
            result.raw = malloc(sizeof(struct dlms_object) * obj->size);
            for(int i = 0; i < obj->size; i++) {
                size += dlms_parse((struct dlms_object*)result.raw + i, ptr + size);
            }
            break;

        case BIT_STRING:
        case OCTET_STRING:
        case VISIBLE_STRING:
        case UTF8_STRING:
            size = obj->size;
            result.raw = ptr;
            break;

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
            result.raw = reverse_bytes(ptr, size);
            break;
    }

    obj->payload = result;
    return size;
}

size_t dlms_parse(struct dlms_object* dest, char* pdu) {
    dest->type = pdu[0];
    dest->size = data_type_len(dest->type);

    bool fixed_size = dest->size || !dest->type;
    uint8_t type_bytes = 1;

    if(!fixed_size) {
        dest->size = pdu[1];
        type_bytes = 2;
    }

    size_t payload_size = dlms_parse_payload(dest, pdu + type_bytes);

    return payload_size + type_bytes;
}