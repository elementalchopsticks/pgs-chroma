#include "util.h"

// TODO: check target endianness
// Currently, the code assumes the target is little-endian

uint8_t read_u8(const uint8_t *buf, size_t *pos) {
    uint8_t val = buf[*pos];
    *pos += 1;
    return val;
}

uint16_t read_u16(const uint8_t *buf, size_t *pos) {
    uint16_t val = buf[*pos + 1] | buf[*pos] << 8;
    *pos += 2;
    return val;
}

uint32_t read_u24(const uint8_t *buf, size_t *pos) {
    uint32_t val = buf[*pos + 2] | buf[*pos + 1] << 8 | buf[*pos] << 16;
    *pos += 3;
    return val;
}

uint32_t read_u32(const uint8_t *buf, size_t *pos) {
    uint32_t val = buf[*pos + 3] | buf[*pos + 2] << 8 | buf[*pos + 1] << 16 | buf[*pos] << 24;
    *pos += 4;
    return val;
}
