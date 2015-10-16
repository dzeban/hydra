include make.config

CFLAGS?=-O2 -g
LDFLAGS?=
LIBS?=

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
INCLUDEDIR?=$(PREFIX)/include
LIBDIR?=$(EXEC_PREFIX)/lib

CFLAGS:=$(CFLAGS) -Wall -Wextra -Iinclude
LIBK_CFLAGS:=$(CFLAGS) -ffreestanding -fbuiltin

CFLAGS:=$(CFLAGS) $(ARCH_CFLAGS)
LIBK_CFLAGS:=$(LIBK_CFLAGS) $(KERNEL_ARCH_CFLAGS)

LIBK_SOURCES:=$(LIBK_OBJS:.o=.c)

.PHONY: install-headers install

$(LIBK): $(LIBK_OBJS)

$(LIBK_OBJS): $(LIBK_SOURCES)

%.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(LIBK_CFLAGS)

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -RTv include $(DESTDIR)$(INCLUDEDIR)

install: $(LIBK)
	mkdir -p $(DESTDIR)$(LIBDIR)
	cp $(LIBK) $(DESTDIR)$(LIBDIR)

clean:
	rm -f $(LIBK)
