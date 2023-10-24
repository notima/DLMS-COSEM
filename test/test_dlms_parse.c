#include "dlms.h"
#include "dlms_crc.h"
#include "test_data.h"
#include "dlms_inspect.h"
#include "assert.h"

#define data test_data

int main(int argc, char* argv) {
    uint16_t crc = (data[sizeof(data) - 3] << 8) | data[sizeof(data) - 2];
    assert_int("crc", crc, (int)dlms_crc(data + 1, sizeof(data) - 4));
    printf(":::TESTING DLMS PARSING:::\n");
    struct dlms_frame frame = dlms_parse_frame(data);
    printf("frame length: %d\ndest address: %x\nsource address: %x\ncontrol bit: %x\n",
        frame.frame_length, frame.dest_address, frame.source_address, frame.control);

    assert_int("frame length", 210, frame.frame_length);
    assert_int("dest address", 0x41, frame.dest_address);
    assert_int("source address", 0x0883, frame.source_address);
    assert_int("control bit", 0x13, frame.control);

    printf("notif tag: %x\nlong invoke id and priority: %x\n",
        frame.data_notification.tag,
        frame.data_notification.long_invoke_id_and_priority);

    inspect_dlms_object(&frame.data_notification.notification_body, 0);

    assert_int("array size", 9, frame.data_notification.notification_body.size);
    dlms_free_object(frame.data_notification.notification_body);
    return 0;
}