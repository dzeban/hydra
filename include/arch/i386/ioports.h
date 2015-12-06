#ifndef _ARCH_I386_IOPORTS_H
#define _ARCH_I386_IOPORTS_H

#include <stdint.h>

void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);

#endif
