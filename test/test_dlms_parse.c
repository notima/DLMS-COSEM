#include "dlms.h"
#include "dlms_crc.h"
#include "test_data.h"
#include "dlms_inspect.h"

int main(int argc, char* argv) {
    printf("%02x", dlms_crc(test_data + 1, sizeof(test_data) - 4));
    printf(":::TESTING DLMS PARSING:::\n");
    struct dlms_object parsed;
    size_t bytes_read = dlms_parse(&parsed, test_data + 18);
    printf("bytes read: %ld\n", bytes_read);
    inspect_dlms_object(&parsed, 0);
    return 0;
}