/* test1.c */

#include <stdio.h>

#include "stdmapper.h"
#include "untar.h"

const char *progname= NULL;

static void Test1(const char *fname);

int main(int argc, char **argv) {
    progname= argv[0];

    if (argc>1) {
        int i;

        for (i=1; i<argc; ++i) {
            Test1(argv[i]);
        }
    } else {
        fprintf(stderr, "usage: %s <filename> ...\n", progname);
    }
    return 0;
}

static void Test1(const char *fname) {
    fprintf(stderr, "Now trying '%s'\n", fname);
}
