/**
 * @file dlms_time.c
 * @author Oliver Norin
 * @brief Converts DLMS time date and date time objects to c struct 
 * 
 * Data formats:
 * 
 * date : OCTET STRING (SIZE(5))
 * {
 *      year highbyte,
 *      year lowbyte,
 *      month,
 *      day of month,
 *      day of week
 * }
 * Where:
 * 
 * year: interpreted as long-unsigned
 *      range 0...big
 *      0xFFFF = not specified
 *      year highbyte and year lowbyte represent the 2 bytes of the long-unsigned
 * 
 * month: interpreted as unsigned
 *      range 1...12, 0xFD, 0xFE, 0xFF
 *      1 is January
 *      0xFD = daylight_savings_end
 *      0xFE = daylight_savings_begin
 *      0xFF = not specified
 * 
 * dayOfMonth: interpreted as unsigned
 *      range 1...31, 0xFD, 0xFE, 0xFF
 *      0xFD = 2nd last day of month
 *      0xFE = last day of month
 *      0xE0 to 0xFC = reserved
 *      0xFF = not specified
 * 
 * dayOfWeek: interpreted as unsigned
 *      range 1...7, 0xFF
 *      1 is Monday
 *      0xFF = not specified
 * 
 * For repetitive dates, the unused parts shall be set to “not specified”.
 * _________
 * 
 * time : OCTET STRING (SIZE(4))
 * {
 *      hour,
 *      minute,
 *      second,
 *      hundredths
 * }
 * Where:
 * 
 * hour: interpreted as unsigned
 *      range 0...23, 0xFF,
 * 
 * minute: interpreted as unsigned
 *      range 0...59, 0xFF,
 * 
 * second: interpreted as unsigned
 *      range 0...59, 0xFF,
 * 
 * hundredths: interpreted as unsigned
 *      range 0...99, 0xFF
 * 
 * For hour, minute, second and hundredths: 0xFF = not specified.
 * For repetitive times the unused parts shall be set to “not specified”.
 * _________
 * 
 * date-time : OCTET STRING (SIZE(12))
 * {
 *      year highbyte,
 *      year lowbyte,
 *      month,
 *      day of month,
 *      day of week,
 *      hour,
 *      minute,
 *      second,
 *      hundredths of second,
 *      deviation highbyte,
 *      deviation lowbyte,
 *      clock status
 * }
 * 
 * The elements of date and time are encoded as defined above. Some may be set
 * to “not specified” as defined above.
 * 
 * In addition:
 * 
 * deviation: interpreted as long
 *      range -720...+720 in minutes of local time to UTC
 *      0x8000 = not specified
 * Deviation highbyte and deviation lowbyte represent the 2 bytes of the long.
 * 
 * Clock_status interpreted as unsigned. The bits are defined as follows:
 * 
 * bit 0 (LSB): invalid a value,
 * bit 1: doubtful b value,
 * bit 2: different clock base c,
 * bit 3: invalid clock status d,
 * bit 4: reserved,
 * bit 5: reserved,
 * bit 6: reserved,
 * bit 7 (MSB): daylight saving active e
 * 0xFF = not specified
 * 
 * // COSEM Interface Classes and OBIS Object Identification System (4.1.6.1)
 */

#include "cosem_time.h"
#include <time.h>

#define DATE_LENGTH 5
#define TIME_LENGTH 4
#define DATETIME_LENGTH 12

void add_time(struct tm* date, unsigned char* raw) {
    date->tm_hour = raw[0];
    date->tm_min = raw[1];
    date->tm_sec = raw[2];
}

void add_date(struct tm* date, unsigned char* raw) {
    int year = (raw[0] << 8) | raw[1];
    date->tm_year = year;
    date->tm_mon = raw[2];
    date->tm_mday = raw[3];
    date->tm_wday = raw[4];
}

struct tm parse_time(struct dlms_object time_object) {
    struct tm result = (struct tm){};

    if(time_object.type != OCTET_STRING) {
        //Incorrect data type 
        return result;
    }

    switch(time_object.size) {
        case TIME_LENGTH:
            // Time object
            add_time(&result, time_object.payload.raw);
            break;

        case DATE_LENGTH:
            // Date object
            add_date(&result, time_object.payload.raw);
            break;

        case DATETIME_LENGTH:
            // Date time object
            add_date(&result, time_object.payload.raw);
            add_time(&result, time_object.payload.raw + DATE_LENGTH);
            break;
    }

    return result;
}