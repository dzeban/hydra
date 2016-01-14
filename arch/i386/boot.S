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

	# Setup kernel stack
	movl $stack_top, %esp

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
