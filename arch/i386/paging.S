#include <arch/i386/mm.h>

.global page_directory_load
page_directory_load:
	pushl %ebp
	movl %esp, %ebp

	movl 8(%ebp), %ebx
	movl %ebx, %cr3

	movl %ebp, %esp
	pop %ebp
	ret

.global cr4_disable_page_size
cr4_disable_page_size:
    pushl %ebp
    movl %esp, %ebp

    movl %cr4, %ebx
    and $(MM_CR4_PS_MASK), %ebx
    movl %ebx, %cr4

    movl %ebp, %esp
    pop %ebp
    ret

.global enable_paging
enable_paging:
	pushl %ebp
	movl %esp, %ebp

	movl %cr0, %ebx
	or  $(MM_CR0_PG | MM_CR0_WP), %ebx
	movl %ebx, %cr0

	movl %ebp, %esp
	pop %ebp
	ret

.global disable_paging
disable_paging:
    pushl %ebp
    movl %esp, %ebp

    movl %cr0, %ebx
    and $(MM_CR0_PG_MASK), %ebx
    movl %ebx, %cr0

    movl %ebp, %esp
    pop %ebp
    ret
