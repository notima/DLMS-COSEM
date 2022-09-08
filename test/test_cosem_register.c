#include "cosem.h"
#include "cosem_units.h"
#include "dlms_inspect.h"
#include "assert.h"

#define VALUE 2500
#define SCALE -1
#define LOGICAL_NAME {1, 0, 32, 7, 0, 255}
#define UNIT 35

unsigned char t_logical_name_payload[] = LOGICAL_NAME;
uint16_t t_payload_value = VALUE;
int8_t t_scale_value = SCALE;
uint8_t t_unit_value = UNIT;

struct dlms_object t_scaler_unit_payload[2];
struct dlms_object t_reg_payload[3];

struct dlms_object get_test_object() {
    struct dlms_object logical_name;
    logical_name.type = OCTET_STRING;
    logical_name.size = sizeof(t_logical_name_payload);
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
    struct dlms_object obj = get_test_object();
    inspect_dlms_object(&obj, 0);
    struct cosem_register reg = cosem_parse_register(obj);
    printf(
        "\nREGISTER:: logical name: %d-%d:%d.%d.%d.%d; value: %f; unit: %s\n", 
        reg.logical_name[0], 
        reg.logical_name[1], 
        reg.logical_name[2], 
        reg.logical_name[3], 
        reg.logical_name[4], 
        reg.logical_name[5],
        (double)(*reg.value.payload.LONG) * pow(10, reg.scaler_unit.scaler),
        cosem_units[reg.scaler_unit.unit]
    );

    assert_str("unit string", "V", cosem_units[reg.scaler_unit.unit]);
    assert_double("scaled value", (double)(*reg.value.payload.LONG) * pow(10, reg.scaler_unit.scaler), (double)VALUE * pow(10, SCALE));
    return 0;
}