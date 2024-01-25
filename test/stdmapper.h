/* stdmapper.h */

#ifndef STDMAPPER_H
#define STDMAPPER_H

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define TAR_MKDIR

class Stream {
private:
    FILE *file;
public:
    Stream(FILE *pfile) { file= pfile; }

    void print(const char *s) {
        fputs(s, file);
    }

    void println(const char *s) {
        fputs(s, file);
        fputc('\n', file);
    }

    void println(unsigned long v) {
        fprintf(file, "%ld", v);
        fputc('\n', file);
    }

    void println() {
        fputc('\n', file);
    }
};

class File: public Stream {
};

Stream Serial(stdout);

#endif
