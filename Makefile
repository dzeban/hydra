ARCHDIR:=arch/$(HOSTARCH)
LIBKDIR:=libk

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot
INCLUDEDIR?=$(PREFIX)/include

include $(ARCHDIR)/make.config
include $(LIBKDIR)/make.config

CFLAGS?=-O2 -g
LDFLAGS?=
LIBS?=

CFLAGS:=$(CFLAGS) -Iinclude -ffreestanding -fbuiltin -Wall -Wextra
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lgcc

CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS:=$(LIBS) $(KERNEL_ARCH_LIBS)

OBJS:=\
$(KERNEL_ARCH_OBJS) \
kernel/kernel.o \

CRTI_OBJ:=$(ARCHDIR)/crti.o
CRTBEGIN_OBJ:=$(shell $(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=crtend.o)
CRTN_OBJ:=$(ARCHDIR)/crtn.o

ALL_OUR_OBJS:=\
$(CRTI_OBJ) \
$(OBJS) \
$(CRTN_OBJ) \

OBJ_LINK_LIST:=\
$(CRTI_OBJ) \
$(CRTBEGIN_OBJ) \
$(OBJS) \
$(CRTEND_OBJ) \
$(CRTN_OBJ) \

LIBK_LINK_OBJS:=$(addprefix libk/, $(LIBK_OBJS))

ISO:=hydra.iso

all: hydra.k

.PHONY: all clean install install-headers install-kernel libk

hydra.k: install-headers libk libk-headers $(OBJ_LINK_LIST) $(ARCHDIR)/linker.ld
	$(CC) -T $(ARCHDIR)/linker.ld -o $@ $(CFLAGS) $(LIBS) $(LIBK_LINK_OBJS) $(OBJ_LINK_LIST) $(LDFLAGS)

libk:
	$(MAKE) -C $(LIBKDIR)

libk-headers:
	$(MAKE) -C $(LIBKDIR) install-headers

%.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(CFLAGS)

%.o: %.S
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f hydra.k $(ISO) $(OBJS) $(ALL_OUR_OBJS) *.o */*.o */*/*.o
	$(MAKE) -C $(LIBKDIR) clean
	rm -rf sysroot
	rm -rf isodir

install: install-headers install-kernel

install-headers: libk-headers
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -RTv include $(DESTDIR)$(INCLUDEDIR)

install-kernel: hydra.k
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp hydra.k $(DESTDIR)$(BOOTDIR)

# Utility targets
iso: hydra.k
	mkdir -p isodir/boot/grub
	cp $^ isodir/boot/
	cp util/grub.cfg isodir/boot/grub/
	grub2-mkrescue -o $(ISO) isodir

geniso: hydra.k
	mkdir -p isodir/boot/grub
	cp $^ isodir/boot/
	cp util/menu.lst isodir/boot/grub/
	cp util/stage2_eltorito isodir/boot/grub
	genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -A os -input-charset utf8 -boot-info-table -o $(ISO) isodir

qemu: geniso
	qemu-system-$(HOSTARCH) -cdrom $(ISO)

qemu-debug: geniso
	qemu-system-$(HOSTARCH) -s -S -no-reboot -no-shutdown -cdrom $(ISO)

bochs: iso
	bochs -f util/bochsrc.txt -q
