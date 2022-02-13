#ifndef PGSC_PGS_H
#define PGSC_PGS_H

#include <stdint.h>
#include <sys/types.h>

enum filter {
    GRAYSCALE,
};

struct pgs {
    uint8_t *data;
    size_t len;
};

ssize_t pgs_open(struct pgs *pgs, const char *path) __attribute__((warn_unused_result));
void pgs_close(struct pgs *pgs);

ssize_t pgs_filter(struct pgs *pgs, enum filter filter) __attribute__((warn_unused_result));
ssize_t pgs_write(const struct pgs *pgs, const char *path) __attribute__((warn_unused_result));

#endif
