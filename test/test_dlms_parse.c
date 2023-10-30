#include "dlms.h"
#include "dlms_crc.h"
#include "test_data.h"
#include "dlms_inspect.h"
#include "assert.h"

struct dlms_frame test_parse(unsigned char* data, uint8_t data_len) {
    uint16_t crc = (data[data_len - 3] << 8) | data[data_len - 2];
    assert_int("crc", crc, (int)dlms_crc(data + 1, data_len - 4));

    struct dlms_frame frame = dlms_parse_frame(data);
    printf("frame length: %d\ndest address: %x\nsource address: %x\ncontrol bit: %x\n",
        frame.frame_length, frame.dest_address, frame.source_address, frame.control);

    printf("notif tag: %x\nlong invoke id and priority: %x\n",
        frame.data_notification.tag,
        frame.data_notification.long_invoke_id_and_priority);

    inspect_dlms_object(&frame.data_notification.notification_body, 0);
    printf("\n");

    return frame;
}

int main(int argc, char* argv) {
    printf(":::TESTING DLMS PARSING:::\n");

    struct dlms_frame frame = test_parse(kaifa_data, sizeof(kaifa_data));

    assert_int("frame length", 154, frame.frame_length);
    assert_int("dest address", 1, frame.dest_address);
    assert_int("source address", 1, frame.source_address);
    assert_int("control bit", 16, frame.control);

    assert_int("array size", 18, frame.data_notification.notification_body.size);
    dlms_free_object(frame.data_notification.notification_body);

    frame = test_parse(kaifa_data2, sizeof(kaifa_data2));

    assert_int("frame length", 39, frame.frame_length);
    assert_int("dest address", 1, frame.dest_address);
    assert_int("source address", 513, frame.source_address);
    assert_int("control bit", 16, frame.control);

    assert_int("array size", 1, frame.data_notification.notification_body.size);
    dlms_free_object(frame.data_notification.notification_body);

    frame = test_parse(kamstrup_data_2, sizeof(kamstrup_data_2));

    assert_int("frame length", 226, frame.frame_length);
    assert_int("dest address", 43, frame.dest_address);
    assert_int("source address", 33, frame.source_address);
    assert_int("control bit", 19, frame.control);

    assert_int("array size", 25, frame.data_notification.notification_body.size);
    dlms_free_object(frame.data_notification.notification_body);

    frame = test_parse(test_data, sizeof(test_data));

    assert_int("frame length", 210, frame.frame_length);
    assert_int("dest address", 65, frame.dest_address);
    assert_int("source address", 2179, frame.source_address);
    assert_int("control bit", 19, frame.control);

    assert_int("array size", 9, frame.data_notification.notification_body.size);
    dlms_free_object(frame.data_notification.notification_body);
    return 0;
}