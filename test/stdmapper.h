/* stdmapper.h */

#ifndef STDMAPPER_H
#define STDMAPPER_H

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define TAR_MKDIR

enum FileState {FiSt_NotOpened, FiSt_OpenFailed, FiSt_PreOpened, FiSt_Opened};

#define FnameNVL(fname) ((fname)? (fname): "[noname]")

class Stream {
private:
    FILE *file;
    char *fname;
    FileState fstate;
    size_t offs;

public:
    Stream(FILE *pfile, const char *pfname, FileState pfstate) {
        file= pfile;              /* ownership taken */
        fname= strdup(pfname);    /* a copy  */
        fstate= pfstate;
        offs= 0;
    }

    Stream() {
        file= NULL;
        fname= NULL;
        fstate= FiSt_NotOpened;
        offs= 0;
    }

    ~Stream() {
        if (fstate==FiSt_Opened) {
            fprintf(stderr, "Stream.destructor *** file '%s' has never been closed\n",
                    FnameNVL(fname));
            fclose(file);
            file= NULL;
            fstate= FiSt_NotOpened;
        }
        if (fname) {
            free(fname);
            fname= NULL;
        }
        offs= 0;
    }

/* more like a 'move' operator */
    Stream& operator=(Stream&& from) {
        if (this->fstate==FiSt_Opened) this->close();
        if (this->fname) free(this->fname);
        if (from.fstate==FiSt_Opened) {
            this->file= from.file;
            this->fname= from.fname;
            this->fstate= FiSt_Opened;
            this->offs= from.offs;
            from.file= NULL;
            from.fname= NULL;
            from.fstate= FiSt_NotOpened;
            from.offs= 0;
        } else {
            if (from.fname) {
                this->fname= strdup(from.fname);
            }
            this->offs= from.offs;
        }
        return *this;
    }

    void begin(int n) { /* Dunno, set bit_per_sec? */
        (void)n;
    }

    size_t readBytes(char *buff, size_t len) {
        size_t rdlen= fread(buff, 1, len, file);
        fprintf(stderr, "readBytes(%d) read %d bytes from file '%s' offset %ld\n",
            (int)len, (int)rdlen, FnameNVL(fname), (long)offs);
        offs += rdlen;
        return rdlen;
    }

    size_t write(unsigned char *buff, size_t len) {
        if (fstate!=FiSt_PreOpened && fstate!=FiSt_Opened) {
            fprintf(stderr, "*** Write to closed file '%s' is not possible (len=%d)\n",
                FnameNVL(fname), (int)len);
            return 0;
        }
        size_t wrlen= fwrite(buff, 1, len, file);
        fprintf(stderr, "write(%d) has written %d bytes into '%s' file offset %ld\n",
            (int)len, (int)wrlen, FnameNVL(fname), (long)offs);
        offs += wrlen;
        return wrlen;
    }

    int close() {
        if (fstate==FiSt_PreOpened) {
            fprintf(stderr, "Stream.close *** don't close file '%s', it is preopened\n",
                    FnameNVL(fname));

        } else if (fstate==FiSt_Opened) {
            fprintf(stderr, "Stream.closing file '%s'-t\n",
                    FnameNVL(fname));
            fclose(file);
            file= NULL;
            fstate= FiSt_NotOpened;
            offs= 0;

        } else if (fstate==FiSt_NotOpened) {
            fprintf(stderr, "Stream.close *** file '%s' is already closed or never has been opened)\n",
                    FnameNVL(fname));
        }
        return 0;
    }

    bool isOpen() {
        return fstate==FiSt_Opened;
    }

    operator bool() {
        return isOpen();
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
    File(FILE *pfile, const char *pfname, FileState pfstate):
      Stream(pfile, pfname, pfstate) {}
};

Stream Serial(stderr, "Serial-stderr", FiSt_PreOpened);

class FS {
public:
    void begin() { /* initialization */
    }

    File open(const char *name, const char *mode) {
        FILE *f= fopen(name, mode);
        if (f==NULL) {
            int ern= errno;
            fprintf(stderr, "*** Error opening file '%s' mode '%s' errno=%d: %s\n",
                    name, mode, ern, strerror(ern));
            return File(f, name, FiSt_NotOpened);
        } else {
            fprintf(stderr, "File '%s' opened for mode '%s'\n",
                    name, mode);
        }
        return File(f, name, FiSt_Opened);
    }

/* Regarding 'already existing directory' */
/* we decide to handle it as non-error */
    int mkdir(const char *pathname, mode_t mode) {
        int rc= ::mkdir(pathname, mode);
        if (rc) {
            int ern= errno;
            if (ern==EEXIST) {
                int rc2;
                struct stat stbuf;
                rc2= stat(pathname, &stbuf);
                if (rc2==0 && S_ISDIR(stbuf.st_mode)) {
                    fprintf(stderr, "Directory '%s' already exists, let's go on\n",
                        pathname);
                    rc= 0;
                }
            }
            if (rc!=0) {
                fprintf(stderr, "*** Error in mkdir '%s' mode 0%o errno=%d: %s\n",
                    pathname, (int)mode, ern, strerror(ern));
            }
        } else {
            fprintf(stderr, "Directory '%s' has been created\n", pathname);
        }
        return rc;
    }
};

FS SPIFFS;

/* No idea, but maybe not absolutely necessary for valgrind-ing */
#define HIGH 1
#define D4   4
#define OUTPUT 1

int  digitalRead(int)       { return 0; }
void digitalWrite(int, int) { return; }
void pinMode(int, int)      { return; }

#endif
