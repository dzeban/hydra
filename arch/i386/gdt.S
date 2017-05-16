.globl gdt_flush
gdt_flush:
    movl 4(%esp), %eax # Load argument
    lgdt (%eax) # Set GDT

    movw $16, %ax # Get GDT[2] (each GDT entry is 8 bytes)
    movw %ax, %ds # Set GDT[2] as a new data segment selector
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss

    jmpl $0x08, $complete_flush # Reload CS with far jmp

complete_flush:
    ret

