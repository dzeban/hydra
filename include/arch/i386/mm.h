#ifndef _ARCH_I386_MM_H
#define _ARCH_I386_MM_H

// CR0-CR4 flags
#define MM_CR0_WP 0x00010000      // Write Protect
#define MM_CR0_PG 0x80000000      // Paging
#define MM_CR4_PS 0x00000010      // Page size extension (4MiB pages)

#define PAGE_SIZE 4096
#define PAGE_SIZE_SHIFT 12

#define PAGE_TABLE_SHIFT 22

#define PDE_PRESENT_FLAG     0x00000001
#define PDE_NOT_PRESENT_MASK ~(PDE_PRESENT)

#define PDE_RW_FLAG     0x00000002
#define PDE_RO_MASK     ~(PDE_RW)

#define PDE_USER_FLAG   0x00000004
#define PDE_SUPERVISOR  ~(PDE_USER)

#define PDE_PAGE_SIZE_FLAG 0x00000080

#define PDE_ENTRIES 1024

// Virtual address to physical address
#define mm_v2p(a) ((a) - __KERNEL_VA_OFFSET)

#ifndef __ASSEMBLER__
uint32_t init_pagedir[PDE_ENTRIES];

extern unsigned long __KERNEL_PA_START;
extern unsigned long __KERNEL_VA_OFFSET;
extern unsigned long __KERNEL_VA_START;
extern unsigned long __KERNEL_VA_END;


void mm_init();

extern void page_directory_load_cr3(unsigned long);
extern void enable_paging();

#endif // __ASSEMBLER__

#endif // _ARCH_I386_MM_H 
