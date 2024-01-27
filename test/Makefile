# test/Makefile

CXXFLAGS := -m64 -g -W -Wall
CPPFLAGS := -I. -I../src/
LDFLAGS  := -m64 -g -L/usr/local/lib64 -Wl,-rpath,/usr/local/lib64

TARGETS := test1 Callback-ESP8266 Extract-ESP8266

all: ${TARGETS}

clean:
	rm -f ${TARGETS} 2>/dev/null || true
	rm -rf data || true

%: %.cc stdmapper.h FS.h ../src/untar.h
	${CXX} ${CXXFLAGS} ${CPPFLAGS} ${LDFLAGS} -o $@ $<

run_test1: test1
	./test1 ../examples/*/data/*.tar

Callback-ESP8266: ../examples/Callback-ESP8266/Callback-ESP8266.ino

run_Callback-ESP8266: Callback-ESP8266
	./Callback-ESP8266 ../examples/Callback-ESP8266/data/test.tar

Extract-ESP8266: ../examples/Extract-ESP8266/Extract-ESP8266.ino

run_Extract-ESP8266: Extract-ESP8266
	./Extract-ESP8266 ../examples/Extract-ESP8266/data/test.tar
