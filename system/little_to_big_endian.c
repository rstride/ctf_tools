#include <stdint.h>

uint32_t swap_endian(uint32_t num) {
    return ((num >> 24) & 0xff) |        // Move byte 3 to byte 0
           ((num << 8) & 0xff0000) |     // Move byte 1 to byte 2
           ((num >> 8) & 0xff00) |       // Move byte 2 to byte 1
           ((num << 24) & 0xff000000);   // Move byte 0 to byte 3
}