#ifndef _ARCH_I386_IOPORTS_H
#define _ARCH_I386_IOPORTS_H

#include <stdint.h>

#define PIC1_CMD 0x20
#define PIC1_DATA 0x21

#define PIC2_CMD 0xA0
#define PIC2_DATA 0xA1

void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);


#endif
