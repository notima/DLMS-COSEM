#include "dlms.h"
#include <stdio.h>
#include <stdint.h>

void print_indentation(uint8_t indentation) {
    for(int i = 0; i < indentation; i++) {
        printf("  ");
    }
}

void inspect_dlms_object(struct dlms_object* object, uint8_t indentation) {
    printf("Type: %02x, Size: %d, Content: ", object->type, object->size);

    switch(object->type) {
        case STRUCT:
            printf("{");
            for(int i = 0; i < object->size; i++) {
                printf("\n");
                print_indentation(indentation + 1);
                printf("[%d]", i);
                inspect_dlms_object(object->payload.STRUCT + i, indentation + 1);
            }
            printf("\n");
            print_indentation(indentation);
            printf("}");
            break;

        case ARRAY:
            printf("{");
            for(int i = 0; i < object->size; i++) {
                printf("\n");
                print_indentation(indentation + 1);
                printf("[%d]", i);
                struct dlms_object element;
                dlms_parse_object_in_array(&element, *object, i);
                inspect_dlms_object(&element, indentation + 1);
            }
            printf("\n");
            print_indentation(indentation);
            printf("}");
            break;

        case BIT_STRING:
        case OCTET_STRING:
            for(int i = 0; i < object->size; i++) {
                printf("%hhu ", object->payload.BIT_STRING[i]);
            }
            break;

        case VISIBLE_STRING:
        case UTF8_STRING:
            printf("%.*s", object->size, object->payload.VISIBLE_STRING);
            break;

        case DOUBLE_LONG_UNSIGNED:
            printf("INT32 %d", *object->payload.DOUBLE_LONG_UNSIGNED);
            break;

        case LONG:
            printf("INT16 %d", *object->payload.LONG);
            break;

        case LONG_UNSIGNED:
            printf("UINT16 %d", *object->payload.LONG_UNSIGNED);
            break;

        case INTEGER:
            printf("INT8 %d", *object->payload.INTEGER);
            break;

        case ENUM:
            printf("ENUM %hhu", *object->payload.UNSIGNED);
    }
}