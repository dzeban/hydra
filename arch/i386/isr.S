.global kbd_isr
kbd_isr:
	call kbd_isr_main
	iret

.extern isr_dispatch

isr_common_stub:
	# Save general-purpose registers in order:
	# EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI.
	pusha

	# Save data segment descriptor
	movw %ds, %ax
	pushl %eax

	# Reload segment registers with kernel code and data descriptors
	#
	# Load kernel data segment descriptor,
	# which is the second one (*8 for byte offset in GDT)
	movw $(0x2 * 8), %ax
	movw %ax, %ds
	movw %ax, %es
	# Load kernel code segment descriptor,
	# which is the first one
	movw $(0x1 * 8), %ax
	movw %ax, %fs
	movw %ax, %gs

	# Right now stack contains:
	#
	# [ss]      -- Saved by CPU only when stack switch occurs,
	# [useresp] -- e.g. interrupted in userspace
	# eflags --+
	# cs       |-- saved implicitly by CPU when interrupt occurs
	# eip    --+
	# errcode
	# irqn
	# eax  --+
	# ecx    |
	# edx    |
	# ebx    |
	# esp    |-- saved by pusha
	# ebp    |
	# esi    |
	# edi  --+
	# ds
	#
	# This is struct regs constructed on stack.
	# We pass it to isr_handler by pushing esp
	pushl %esp
	call isr_dispatch
	addl $4, %esp # Clean passed struct regs

	# Reload original data segment descriptor
	popl %eax
	movw %ax, %ds 
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs

	popa
	# Clean up pushed error code
	# and irq number
	add $8, %esp 

	# Enable interrupts and return
	sti
	iret

# define a macro, taking one parameter
.macro ISR_NOERRCODE n  
.global isr\n
isr\n:
	cli
	# When no error code provided we explicitly push 0 to unify ISR stacks of
	# ISR_NOERRCODE and ISR_ERRCODE
	pushl $0
	pushl $\n
	jmp isr_common_stub
.endm

.macro ISR_ERRCODE n
.global isr\n
isr\n:
	cli
	# Error code already pushed before interrupt vector number
	pushl \n
	jmp isr_common_stub
.endm

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE 8
ISR_NOERRCODE 9
ISR_ERRCODE 10
ISR_ERRCODE 11
ISR_ERRCODE 12
ISR_ERRCODE 13
ISR_ERRCODE 14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

ISR_NOERRCODE 33

