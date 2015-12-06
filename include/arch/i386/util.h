#ifndef _ARCH_I386_UTIL_H
#define _ARCH_I386_UTIL_H

/** Issue xchg %bx %bx - bochs magic breakpoint */
void __BREAK_BOCHS(void);

#endif
