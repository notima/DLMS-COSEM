#ifdef __cplusplus
extern "C" {
#endif

#include "dlms.h"
#include <stdint.h>

struct cosem_scaler_unit {
    int8_t scaler;
    uint8_t unit;
};

struct cosem_data {
    unsigned char* logical_name;
    struct dlms_object value;
};

struct cosem_register {
    unsigned char* logical_name;
    struct dlms_object value;
    struct cosem_scaler_unit scaler_unit;
};

/**
 * Calculate the real value of a register by scaling it
 * according to the scaler value in the scaler_unit struct
 * 
 * @return double v * 10 ^ s (where v is value and s is scaler)
 */
double cosem_get_scaled_value(struct cosem_register);

struct cosem_data cosem_parse_data(struct dlms_object);
struct cosem_scaler_unit cosem_parse_scaler_unit(struct dlms_object);
struct cosem_register cosem_parse_register(struct dlms_object);

#ifdef __cplusplus
}
#endif