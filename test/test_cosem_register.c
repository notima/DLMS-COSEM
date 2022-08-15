#include "cosem.h"
#include "cosem_units.h"
#include "dlms_inspect.h"

unsigned char t_logical_name_payload[] = {1, 0, 32, 7, 0, 255};
uint16_t t_payload_value = 2500;
int8_t t_scale_value = -1;
uint8_t t_unit_value = 35;

struct dlms_object t_scaler_unit_payload[2];
struct dlms_object t_reg_payload[3];

struct dlms_object get_test_object() {
    struct dlms_object logical_name;
    logical_name.type = OCTET_STRING;
    logical_name.size = 6;
    logical_name.payload.OCTET_STRING = t_logical_name_payload;
    t_reg_payload[0] = logical_name;

    struct dlms_object value;
    value.type = LONG;
    value.size = 2;
    value.payload.LONG = &t_payload_value;
    t_reg_payload[1] = value;

    struct dlms_object scale;
    scale.type = INTEGER;
    scale.size = 1;
    scale.payload.INTEGER = &t_scale_value;
    t_scaler_unit_payload[0] = scale;

    struct dlms_object unit;
    unit.type = ENUM;
    unit.size = 1;
    unit.payload.UNSIGNED = &t_unit_value;
    t_scaler_unit_payload[1] = unit;

    struct dlms_object scal_unit;
    scal_unit.type = STRUCT;
    scal_unit.size = 2;
    scal_unit.payload.STRUCT = t_scaler_unit_payload;
    t_reg_payload[2] = scal_unit;

    struct dlms_object obj;
    obj.type = STRUCT;
    obj.size = 3;
    obj.payload.STRUCT = t_reg_payload;

    return obj;
}

int main(int argc, char* argv) {
    struct dlms_object reg = get_test_object();
    inspect_dlms_object(&reg, 0);
    struct cosem_register creg = cosem_parse_register(reg);
    printf(
        "\nREGISTER:: logical name: %d-%d:%d.%d.%d.%d; value: %f; unit: %s\n", 
        creg.logical_name[0], 
        creg.logical_name[1], 
        creg.logical_name[2], 
        creg.logical_name[3], 
        creg.logical_name[4], 
        creg.logical_name[5],
        (double)(*creg.value.LONG) * pow(10, creg.scaler_unit.scaler),
        cosem_units[creg.scaler_unit.unit]
    );
    return 0;
}