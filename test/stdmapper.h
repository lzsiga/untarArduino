/* stdmapper.h */

#ifndef STDMAPPER_H
#define STDMAPPER_H

#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define TAR_MKDIR

class Stream {
private:
    FILE *file;
    char *fname;
    bool preopened;

public:
    Stream(FILE *pfile, const char *pfname, bool ppreopened) {
        file= pfile;              /* ownership taken */
        fname= strdup(pfname);    /* a copy  */
        preopened= ppreopened;
    }

    Stream() {
        file= NULL;
        fname= NULL;
        preopened= 0;
    }

    ~Stream() {
        if (!preopened && file) {
            fprintf(stderr, "Stream.destructor *** file '%s' has never been closed\n",
                    fname? fname: "[noname]");
            fclose(file);
            file= NULL;
        }
        if (fname) {
            free(fname);
            fname= NULL;
        }
    }

    size_t readBytes(char *buff, size_t len) {
        size_t rdlen= fread(buff, 1, len, file);
        fprintf(stderr, "readBytes(%d) read %d bytes\n", (int)len, (int)rdlen);
        return rdlen;
    }

    size_t write(unsigned char *buff, size_t len) {
        size_t wrlen= fwrite(buff, 1, len, file);
        fprintf(stderr, "write(%d) has written %d bytes\n", (int)len, (int)wrlen);
        return wrlen;
    }

    int close() {
        if (preopened) {
            fprintf(stderr, "Stream.close *** don't close file '%s', it is preopened\n",
                    fname? fname: "[noname]");
        } else {
            if (file) {
                fprintf(stderr, "Stream.closing file '%s'-t\n",
                        fname? fname: "[noname]");
                fclose(file);
                file= NULL;
            } else {
                if (fname) {
                    fprintf(stderr, "Stream.close *** file '%s' has already been closed\n",
                        fname? fname: "[noname]");
                }
            }
        }
        return 0;
    }

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
public:
    File():Stream() {}
    File(FILE *pfile, const char *pfname, bool ppreopened):
      Stream(pfile, pfname, ppreopened) {}
};

Stream Serial(stdout, "Serial-stdout", true);

class FS {
public:
    File open(const char *name, const char *mode) {
        FILE *f= fopen(name, mode);
        if (f==NULL) {
            int ern= errno;
            fprintf(stderr, "Error opening file '%s' mode '%s' errno=%d: %s\n",
                    name, mode, ern, strerror(ern));
            exit(1);
        }
        return File(f, name, false);
    }
};

FS SPIFFS;

#endif
