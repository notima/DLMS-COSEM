# DLMS-COSEM C-Library

This library is used for parsing DLMS/COSEM APDUs

The following COSEM classes are currently supported:

- Data (class_id 1)
- Register (class_id 3)

## Usage

See the tests for example usage

## Testing

```bash
cd ./test
cmake . -B build
cmake --build build
cd build
ctest --verbose 
```