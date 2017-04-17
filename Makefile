PROGNAME := mach-osx
CC ?= cc

CFLAGS := -g
CFILES := $(wildcard *.c)
OFILES := $(patsubst %.c,%.o,$(CFILES))

.PHONY: all clean

all: $(PROGNAME)

$(PROGNAME): $(OFILES)
	$(CC) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OFILES) $(PROGNAME)
