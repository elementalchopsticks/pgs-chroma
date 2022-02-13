#ifndef PGSC_UTIL_H
#define PGSC_UTIL_H

#include <stddef.h>
#include <stdint.h>

// Helper functions to read big-endian values from buffer
uint8_t read_u8(const uint8_t *buf, size_t *pos);
uint16_t read_u16(const uint8_t *buf, size_t *pos);
uint32_t read_u24(const uint8_t *buf, size_t *pos);
uint32_t read_u32(const uint8_t *buf, size_t *pos);

#endif
