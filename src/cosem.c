#include "cosem.h"
#include <math.h>

struct cosem_data cosem_parse_data(struct dlms_object obj) {
    struct cosem_data result;
    result.logical_name = obj.payload.STRUCT[0].payload.OCTET_STRING;
    result.value = obj.payload.STRUCT[1].payload;
}

struct cosem_register cosem_parse_register(struct dlms_object obj) {
    struct cosem_register result;
    result.logical_name = obj.payload.STRUCT[0].payload.OCTET_STRING;
    result.value = obj.payload.STRUCT[1].payload;
    result.scaler_unit = cosem_parse_scaler_unit(obj.payload.STRUCT[2]);
}

struct cosem_scaler_unit cosem_parse_scaler_unit(struct dlms_object obj) {
    struct cosem_scaler_unit result;
    result.scaler = *obj.payload.STRUCT[0].payload.INTEGER;
    result.unit = *obj.payload.STRUCT[1].payload.UNSIGNED;
}

double cosem_get_scaled_value(struct cosem_register reg) {
    double value = *reg.value.LONG64 * pow(10, reg.scaler_unit.scaler);
    return value;
}