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

    File f= SPIFFS.open(fname, "r");	// Open source file
}
#if 0
Tar<FS> tar(&SPIFFS);			// Declare and initialize class passing pointer to SPIFFS as target filesystem

void setup() {
  Serial.begin(74880);
  SPIFFS.begin();			// Initialize SPIFFS
  File f = SPIFFS.open(FILENAME, "r");	// Open source file
  if (f) {
    tar.open((Stream*)&f);		// Pass source file as Stream to Tar object
    tar.dest("/");			// Set destination prefix to append to all files in archive. Should start with "/" for SPIFFS
    tar.extract();			// Process with extraction
  } else {
    Serial.println("Error open .tar file");
  }
}
#endif
