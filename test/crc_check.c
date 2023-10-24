#include <stdlib.h>
#include <stdio.h>
#include <dlms_crc.h>

int main(int argc, char* argv[]) {
    unsigned char bytes[argc-1];
    for(int i = 1; i < argc; i++) {
        bytes[i-1] = strtol(argv[i], NULL, 16);
    }

    uint16_t crc = dlms_crc(bytes, argc-1);

    printf("CRC: %04x (%d)\n", crc, crc);
}