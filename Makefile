.PHONY : all

CC=g++
CPPFLAGS=-fPIC -I
LDFLAGS=-shared

SOURCES=src/trace.cpp
INCDIR=inc/

TARGET=lib/libtrace.so

all: $(TARGET)

clean:
	rm -f $(TARGET)

$(TARGET):
	$(CC) $(CPPFLAGS) $(INCDIR) $(SOURCES) -o $@ $(LDFLAGS)
