# DLMS-COSEM C-Library

This library is used for parsing DLMS/COSEM messages

The following COSEM classes are currently supported:

- Data (class_id 1)
- Register (class_id 3)

## Usage

The following is an example of what a data frame from an AIDON smart meter might look like:

    7e a0d2 41 0883 13 82d6 e6e700
        0f 40000000 00
        0109
            0202 0906 0101000281ff 0a0b 4149444f4e5f5630303031
            0202 0906 0000600100ff 0a10 37333539393932383930393431373432
            0202 0906 0000600107ff 0a04 36353135
            0203 0906 0100010700ff 06 00000552 0202 0f00 161b
            0203 0906 0100020700ff 06 00000000 0202 0f00 161b
            0203 0906 0100030700ff 06 000003e4 0202 0f00 161d
            0203 0906 0100040700ff 06 00000000 0202 0f00 161d
            0203 0906 01001f0700ff 10 005d 0202 0fff 1621
            0203 0906 0100200700ff 12 09c4 0202 0fff 1623
    e0c4 7e

### CRC calculation

Calculating the crc checksum of a data buffer:

```C
uint16_t crc = dlms_crc(buf + 1, sizeof(buf) - 4);
```

Since every data frame starts and ends with '~' (`7e`) and these bytes are not taken into account when calculating the checksum, we need to offset by one byte at the beginning of the buffer and offset by 3 at the end (the buffer ends with the senders checksum before the end flag). So in other words, the bytes passed to the `dlms_crc` function are the bytes after `7e` and before `e0c4 7e` in the example above. Using the AIDON example data, the expected return value is of course `e0c4`.

### Parsing payload

Once we have verified the checksum and found the beginning of the payload, parsing it is very straight forward:

```C
struct dlms_object parsed;
size_t bytes_read = dlms_parse(&parsed, test_data + header_len);
```

Since the payload value can come in various data types, a `union` is used to represent the value. This means that we can check what data type to expect and retrieve the data in that data type. For example:

```C
if(parsed.type == INTEGER) {
    int8_t value = *parsed.payload.INTEGER;
}
```
Keep in mind that the payload is always a pointer.

payloads of type ARRAY are not parsed automatically, instead, every element has to be parsed separately using `dlms_parse_object_in_array(struct dlms_object* dest, struct dlms_object array, uint8_t index)`.
For example:

```C
struct dlms_object element;
dlms_parse_object_in_array(&element, *array, 15);
```

**!!!WARNING!!!**

When parsing dlms objects of type `ARRAY` (1) or `STRUCT` (2), the payload will point to new dlms objects allocated on the heap. this means that they need to be freed manually in order to avoid memory leaks. Since arrays and structs can contain nested arrays and structs, this can become a bit complicated. Therefore, we have the `dlms_free` method to help us free all allocated memory recursively.

Example:

```C
dlms_free(parsed);
```

### Parsing COSEM registers

If a dlms object is expected to be a COSEM register, it can be parsed as such.

```C
struct cosem_register reg = cosem_parse_register(obj);
```

Keep in mind that before using the value in a COSEM register, it has to be scaled according to the scaler value. The scaler value is the exponent (to the base of 10) of the multiplication factor. For example:

```C
double scaled_value = (double)(*reg.value.LONG) * pow(10, reg.scaler_unit.scaler);
```

The Unit enum can be converted to a unit string by retrieving the element of the `cosem_units` array in `cosem_units.h` that corresponds to the enum value. For example:

```C
char* unit_str = cosem_units[reg.scaler_unit.unit];
```

## Testing
How to compile and run the unit tests:

```bash
cd ./test
cmake . -B build
cmake --build build
cd build
ctest --verbose 
```