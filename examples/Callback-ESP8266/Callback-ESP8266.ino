/*
 * This code based on
 * 
 * "untar" is an extremely simple tar extractor
 *
 * Written by Tim Kientzle, March 2009.
 *
 * Released into the public domain.
 *
 * Ported to Arduino library by Alexander Emelainov (a.m.emelianov@gmail.com), August 2017
 *  https://github.com/emelianov/untarArduino
 *
 */

/*
 * Callback demo. Skips all files except 'data/create.txt' and print extraction process messages from callback not from library main code.
 *
 */

#include <FS.h>
#define TAR_CALLBACK
#define TAR_SILENT
#include <untar.h>

#define PIN D4

#ifndef FILENAME
#define FILENAME "/test.tar"
#endif
#define EXTRACT "data/create.txt"

Tar<FS> tar(&SPIFFS);
bool fWrite = false;
const char* filename = EXTRACT;

bool printFile(const char* name) {
	Serial.print(name);
	if (strcmp(name, filename) == 0) {
		Serial.println();
		fWrite = true;
		return true;
	}
	Serial.println(" -  SKIP");
	return false;
}

void blinkWrite(const char* data, size_t s) {
	(void)data;
	(void)s;
	if (!fWrite) return;
	digitalWrite(PIN, !digitalRead(PIN));
}

void eof() {
  digitalWrite(PIN, HIGH);  
	fWrite = false;
}

void setup() {
  Serial.begin(74880);
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, HIGH);
  SPIFFS.begin();
  tar.onFile((cbTarProcess)printFile);
  tar.onData((cbTarData)blinkWrite);
  tar.onEof(eof);
  File f = SPIFFS.open(FILENAME, "r");
  if (f) {
    tar.open((Stream*)&f);
    tar.dest("/");
	  tar.extract();
  } else {
    Serial.println("Error open .tar file");
  }
}

void loop() {

}
