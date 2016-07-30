.global page_directory_load_cr3
page_directory_load_cr3:
	pushl %ebp
	movl %esp, %ebp

	movl 8(%ebp), %ebx
	movl %ebx, %cr3

	movl %ebp, %esp
	pop %ebp
	ret

.global enable_paging
enable_paging:
	pushl %ebp
	movl %esp, %ebp

	movl %cr0, %ebx
	or  $0x80000000, %ebx
	movl %ebx, %cr0

	movl %ebp, %esp
	pop %ebp
	ret
