CC      := gcc
CFLAGS  := -Wall -Wextra -O2 -lsodium
PREFIX  := /usr/local
BINDIR  := $(PREFIX)/bin

TARGET  := snippethold
SRCS    := main.c snippethold.c
OBJS    := $(SRCS:.c=.o)
HEADERS := snippethold.h

.PHONY: all build clean install uninstall

all: build

build: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

install: build
	mkdir -p $(DESTDIR)$(BINDIR)
	install -m 0755 $(TARGET) $(DESTDIR)$(BINDIR)/$(TARGET)

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/$(TARGET)
