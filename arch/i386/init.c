#include <arch/init.h>
#include <arch/i386/segment.h>
#include <arch/i386/interrupts.h>

#include <arch/i386/util.h>

void arch_initialize(void)
{
	gdt_init();
	__BREAK_BOCHS();
	interrupts_init();
}
