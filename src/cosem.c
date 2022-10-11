#include "cosem.h"
#include <math.h>

#include <stdio.h>

struct cosem_data cosem_parse_data(struct dlms_object obj) {
    struct cosem_data result;
    result.logical_name = obj.payload.OBJECT[0].payload.OCTET_STRING;
    result.value = obj.payload.OBJECT[1];
    return result;
}

struct cosem_register cosem_parse_register(struct dlms_object obj) {
    struct cosem_register result;
    result.logical_name = obj.payload.OBJECT[0].payload.OCTET_STRING;
    result.value = obj.payload.OBJECT[1];
    result.scaler_unit = cosem_parse_scaler_unit(obj.payload.OBJECT[2]);
    return result;
}

struct cosem_scaler_unit cosem_parse_scaler_unit(struct dlms_object obj) {
    struct cosem_scaler_unit result;
    result.scaler = *obj.payload.OBJECT[0].payload.INTEGER;
    result.unit = *obj.payload.OBJECT[1].payload.UNSIGNED;
    return result;
}

double cosem_get_scaled_value(struct cosem_register reg) {
    double value = *reg.value.payload.LONG64 * pow(10, reg.scaler_unit.scaler);
    return value;
}