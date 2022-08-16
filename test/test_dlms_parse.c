#include "dlms.h"
#include "dlms_crc.h"
#include "test_data.h"
#include "dlms_inspect.h"
#include "assert.h"

int main(int argc, char* argv) {
    assert_int("crc", 0xe0c4, (int)dlms_crc(test_data + 1, sizeof(test_data) - 4));
    printf(":::TESTING DLMS PARSING:::\n");
    struct dlms_object parsed;
    size_t bytes_read = dlms_parse(&parsed, test_data + 18);
    printf("bytes read: %ld\n", bytes_read);
    inspect_dlms_object(&parsed, 0);
    assert_int("array size", 9, parsed.size);
    dlms_free(parsed);
    return 0;
}