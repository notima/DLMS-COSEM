#include "test.h"
#include "cosem_time.h"

char data[] = {0x07, 0xe4, 0x01, 0x19, 0x06, 0x0e, 0x00, 0x0a, 0xff, 0x80, 0x00, 0x00};

void main() {
    printf("Testing COSEM Time parsing\n");

    union dlms_payload payload;
    payload.raw = data;
    struct dlms_object object;
    object.size = 12;
    object.type = OCTET_STRING;
    object.payload = payload;
    
    struct tm time = parse_time(object);

    printf(
            "%02d/%02d/%02d %02d:%02d:%02d\n",
            time.tm_year,
            time.tm_mon,
            time.tm_mday,
            time.tm_hour,
            time.tm_min,
            time.tm_sec
        );
}