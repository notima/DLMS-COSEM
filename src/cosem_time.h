#ifndef DLMS_TIME_H
#define DLMS_TIME_H
#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>
#include "dlms.h"

struct tm parse_time(struct dlms_object time_object);

#ifdef __cplusplus
}
#endif
#endif