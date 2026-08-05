CC      := gcc
CFLAGS  := -Wall -Wextra -O2
PREFIX  := /usr/local
BINDIR  := $(PREFIX)/bin

TARGET  := clipstore
SRCS    := main.c clipstore.c
OBJS    := $(SRCS:.c=.o)
HEADERS := clipstore.h

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
