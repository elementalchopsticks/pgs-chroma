#include "pgs.h"
#include "util.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const uint8_t PALETTE_DEFINITION = 0x14;

struct pixel {
    uint8_t y;
    uint8_t cr;
    uint8_t cb;
    uint8_t a;
};

ssize_t pgs_open(struct pgs *pgs, const char *path) {
    FILE *fp = fopen(path, "rb");
    if (!fp) {
        fprintf(stderr, "Failed to open '%s': %s\n", path, strerror(errno));
        return -1;
    }

    if (fseek(fp, 0, SEEK_END) != 0) {
        fprintf(stderr, "Failed to seek '%s': %s\n", path, strerror(errno));
        fclose(fp);
        return -1;
    }

    int len;
    if ((len = ftell(fp)) < 0) {
        fprintf(stderr, "Failed to determine length of '%s': %s\n", path, strerror(errno));
        fclose(fp);
        return -1;
    }

    rewind(fp);

    uint8_t *data = calloc(len, 1);
    if (fread(data, 1, len, fp) != (size_t)len) {
        fprintf(stderr, "Failed to read '%s': %s\n", path, strerror(errno));
        free(data);
        fclose(fp);
        return -1;
    }

    fclose(fp);

    pgs->data = data;
    pgs->len = len;

    return 0;
}

void pgs_close(struct pgs *pgs) { free(pgs->data); }

static struct pixel grayscale_filter(struct pixel p) {
    p.cr = 128;
    p.cb = 128;
    return p;
}

ssize_t pgs_filter(struct pgs *pgs, enum filter filter) {
    struct pixel (*fn)(struct pixel);
    switch (filter) {
    case GRAYSCALE:
        fn = grayscale_filter;
        break;
    };

    size_t pos = 0;
    while (pos < pgs->len) {
        uint8_t magic[2] = {read_u8(pgs->data, &pos), read_u8(pgs->data, &pos)};
        __attribute__((unused)) uint32_t pts = read_u32(pgs->data, &pos);
        __attribute__((unused)) uint32_t dts = read_u32(pgs->data, &pos);
        uint8_t type = read_u8(pgs->data, &pos);
        uint16_t size = read_u16(pgs->data, &pos);

        if (magic[0] != 0x50 || magic[1] != 0x47) {
            fprintf(stderr, "Invalid magic bytes: 0x%02X%02X\n", magic[0], magic[1]);
            return -1;
        }

        size_t next_seg = pos + size;

        if (type == PALETTE_DEFINITION) {
            __attribute__((unused)) uint8_t palette_id = read_u8(pgs->data, &pos);
            __attribute__((unused)) uint8_t palette_version = read_u8(pgs->data, &pos);
            while (pos < next_seg) {
                __attribute__((unused)) uint8_t entry_id = read_u8(pgs->data, &pos);
                uint8_t y = read_u8(pgs->data, &pos);
                uint8_t cr = read_u8(pgs->data, &pos);
                uint8_t cb = read_u8(pgs->data, &pos);
                uint8_t a = read_u8(pgs->data, &pos);

                struct pixel p = {
                    .y = y,
                    .cr = cr,
                    .cb = cb,
                    .a = a,
                };
                p = fn(p);

                pgs->data[pos - 4] = p.y;
                pgs->data[pos - 3] = p.cr;
                pgs->data[pos - 2] = p.cb;
                pgs->data[pos - 1] = p.a;
            }
        }

        pos = next_seg;
    }

    return 0;
}

ssize_t pgs_write(const struct pgs *pgs, const char *path) {
    FILE *fp = fopen(path, "wb");
    if (!fp) {
        fprintf(stderr, "Failed to open '%s': %s\n", path, strerror(errno));
        return -1;
    }

    if (fwrite(pgs->data, 1, pgs->len, fp) != pgs->len) {
        fprintf(stderr, "Failed to write to '%s': %s\n", path, strerror(errno));
        fclose(fp);
        return -1;
    }

    fclose(fp);

    return 0;
}
