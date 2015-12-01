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
	unsigned int eax, ebx, ecx, edx;

	__get_cpuid(1, &eax, &ebx, &ecx, &edx);

	printf("CPUID: %u %u %u %u!\n", eax, ebx, ecx, edx);
	printf("APIC: %d\n", (edx >> 9) & 0x1);
}
