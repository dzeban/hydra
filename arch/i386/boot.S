#include <arch/i386/mm.h>

# Declare constants used for creating a multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Reserve a stack for the initial thread.
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# can't use linker constant here, so hard-coded "0xc0100000"
.set KERNEL_VIRTUAL_PAGE, (0xc0100000 >> PAGE_DIR_SHIFT)
.set INIT_PDE, (PDE_PRESENT_FLAG | PDE_RW_FLAG | PDE_PAGE_SIZE_FLAG)

.section .data
.align PAGE_SIZE
.global init_pagedir
init_pagedir:
    # Page 0 is mapped to frame 0
    .long INIT_PDE
    .fill (KERNEL_VIRTUAL_PAGE - 1), 4, 0

    # Page 768 = (0xc0100000 >> PAGE_DIR_SHIFT) is also mapped to frame 0.
    .long INIT_PDE
    .fill (PDE_ENTRIES - KERNEL_VIRTUAL_PAGE - 1), 4, 0

# The kernel entry point.
# The linker script specifies _start as the entry point to the kernel and the
# bootloader will jump to this position once the kernel has been loaded. It
# doesn't make sense to return from this function as the bootloader is gone.
#
# IMPORTANT: Multiboot comliant bootloader sets machine state to protected mode,
# as per specification, section 3.2 "Machine state":
# > ‘CR0’
# >    Bit 31 (PG) must be cleared. Bit 0 (PE) must be set. Other bits are all
# >    undefined. 
#
.section .text
.global _start
.type _start, @function
_start:
    # Disable interrupts, until we setup IDT
    cli

    # WARNING: Don't use eax and ebx registers here
    # because they contain multiboot info.
    # We're gonna pass that multiboot info to kernel_early as arguments

    # Enable 4 MiB pages to have single page table
    # during kernel initialization
    movl %cr4, %edx
    orl $(MM_CR4_PS), %edx
    movl %edx, %cr4

    # Setup kernel init page directory (see linker.ld)
    movl $(init_pagedir - 0xc0000000) , %edx
    movl %edx, %cr3

    # Enable paging
    movl %cr0, %edx
    orl $(MM_CR0_PG|MM_CR0_WP), %edx
    movl %edx, %cr0

    mov $_high_start, %edx
    jmp *%edx

.global _high_start
_high_start:
    # Setup kernel stack
    movl $stack_top, %esp

    # Multiboot bootloader set %eax to magic 0X2BADB002 and %ebx to contain the
    # 32-bit physical address of the multiboot information structure.
    # We pass it to kernel_early to detect available memory and verify we're
    # loaded correctly
    pushl %ebx
    pushl %eax

    # Initialize the core kernel before running the global constructors.
    call kernel_early

    # Call the global constructors.
    call _init

    # Transfer control to the main kernel.
    call kernel_main

# Hang if kernel_main unexpectedly returns.
.Lhang:
    hlt
    jmp .Lhang
.size _start, . - _start
