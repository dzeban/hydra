#include <stdio.h>
#include <kernel/tty.h>
#include <arch/init.h>
#include <cpuid.h>

void kernel_early(void)
{
	terminal_initialize();
	arch_initialize();
}

void kernel_main(void)
{
	printf("Hello\n");
}
