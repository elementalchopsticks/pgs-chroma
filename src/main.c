#include "pgs.h"
#include "util.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
    struct {
        unsigned int h : 1;
        unsigned int v : 1;
        const char *f;
    } flags;

    memset(&flags, 0, sizeof flags);

    int opt;
    while ((opt = getopt(argc, argv, "f:hv")) != -1) {
        switch (opt) {
        case 'f':
            flags.f = optarg;
            break;
        case 'h':
            flags.h = 1;
            break;
        case 'v':
            flags.v = 1;
            break;
        default:
            return 1;
        }
    }

    if (flags.h) {
        printf("Usage: pgs-chroma [options...] <input> <output>\n\n"
               "Options:\n"
               "    -f   Specify filter to be used. Default: 'clipped'.\n"
               "             Options: 'bw', 'clipped', 'grayscale'.\n"
               "    -h   Show help message and exit.\n"
               "    -v   Print version information and exit.\n");
        return 0;
    } else if (flags.v) {
        printf("pgs-chroma v%s\n", PROJECT_VERSION);
        return 0;
    }

    enum filter filter;
    if (!flags.f) {
        filter = FILTER_CLIPPED;
    } else {
        if (strcmp(flags.f, "bw") == 0) {
            filter = FILTER_BW;
        } else if (strcmp(flags.f, "clipped") == 0) {
            filter = FILTER_CLIPPED;
        } else if (strcmp(flags.f, "grayscale") == 0) {
            filter = FILTER_GRAYSCALE;
        } else {
            fprintf(stderr, "Invalid filter: '%s'\n", flags.f);
            return 1;
        }
    }

    if (optind + 2 > argc) {
        fprintf(stderr, "Not enough arguments\n");
        return 1;
    }

    const char *input = argv[optind];
    const char *output = argv[optind + 1];

    struct pgs pgs;
    if (pgs_open(&pgs, input) != 0) {
        return 1;
    }

    if (pgs_filter(&pgs, filter) != 0) {
        pgs_close(&pgs);
        return 1;
    }

    if (pgs_write(&pgs, output) != 0) {
        pgs_close(&pgs);
        return 1;
    }

    pgs_close(&pgs);

    return 0;
}
