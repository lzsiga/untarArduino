/* test1.c */

#include <stdio.h>

#include "stdmapper.h"
#include "untar.h"

static struct {
    const char *progname;
    const char *prefix;
} var= {
    NULL,
    "./"
};

static void Test1(const char *fname);
static void ParseArgs(int *pargc, char ***pargv);

int main(int argc, char **argv) {
    ParseArgs(&argc, &argv);

    if (argc>1) {
        int i;

        for (i=1; i<argc; ++i) {
            Test1(argv[i]);
        }
    } else {
        fprintf(stderr, "usage: %s <filename> ...\n", var.progname);
    }
    return 0;
}

static void Test1(const char *fname) {
    fprintf(stderr, "Now trying '%s'\n", fname);

    Tar<FS> tar(&SPIFFS);

    File f= SPIFFS.open(fname, "r");
    if (!f) {
        return;
    }
    tar.open(&f);
    tar.dest(var.prefix);
    tar.extract();
    if (f) f.close();
}

static void ParseArgs (int *pargc, char ***pargv)
{
    char *arg;
    int argc;
    char **argv;

    argc = *pargc;
    argv = *pargv;

    var.progname = argv[0];

    while (--argc>0 && **++argv=='-') {
        arg = argv[0];
        switch (arg[1]) {
        case 0: case '-':
            --argc, ++argv;
            goto NO_MORE_OPT;

        case 'p': case 'P':
            if (strcasecmp (argv[0], "-prefix")==0) {
                if (argc<2) goto OPTNVAL;
                --argc;
                ++argv;
                var.prefix= argv[0][0] ? argv[0]: NULL;
                break;

            } else goto UNKOPT;

        default:
UNKOPT:     fprintf (stderr, "Unknown option: '%s'\n", arg);
            exit (12);
OPTNVAL:    fprintf (stderr, "No value for option '%s'\n", arg);
            exit (12);
        }
    }
NO_MORE_OPT:
    ++argc, --argv;

    *pargc = argc;
    *pargv = argv;
}
