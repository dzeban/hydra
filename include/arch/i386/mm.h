#ifndef _ARCH_I386_MM_H
#define _ARCH_I386_MM_H

// CR0-CR4 flags
#define MM_CR0_WP 0x00010000      // Write Protect
#define MM_CR0_PG 0x80000000      // Paging
#define MM_CR0_PG_MASK ~(MM_CR0_PG)
#define MM_CR4_PS 0x00000010      // Page size extension (4MiB pages)
#define MM_CR4_PS_MASK ~(MM_CR4_PS)

#define PAGE_SIZE 4096
#define PAGE_SIZE_SHIFT 12
#define PAGE_MASK 0xFFF

#define PAGE_TABLE_SIZE 1024
#define PAGE_TABLE_SHIFT 12
#define PAGE_TABLE_MASK 0x3FF

#define PAGE_DIR_SIZE 1024
#define PAGE_DIR_SHIFT 22
#define PAGE_DIR_MASK 0x3FF

#define PDX(a) ((a >> PAGE_DIR_SHIFT)   & PAGE_DIR_MASK)
#define PTX(a) ((a >> PAGE_TABLE_SHIFT) & PAGE_TABLE_MASK)


// PDE, PTE FLAGS
#define PDE_PRESENT_FLAG     0x00000001
#define PDE_NOT_PRESENT_MASK ~(PDE_PRESENT)

#define PTE_PRESENT_FLAG     0x00000001
#define PTE_NOT_PRESENT_MASK ~(PTE_PRESENT)

#define PDE_RW_FLAG     0x00000002
#define PDE_RO_MASK     ~(PDE_RW)

#define PTE_RW_FLAG     0x00000002
#define PTE_RO_MASK     ~(PTE_RW)

#define PDE_USER_FLAG   0x00000004
#define PDE_SUPERVISOR  ~(PDE_USER)

#define PTE_USER_FLAG   0x00000004
#define PTE_SUPERVISOR  ~(PTE_USER)

#define PDE_PAGE_SIZE_FLAG 0x00000080

#define PTE_ADDR(pte) (pte & ~PAGE_MASK)

// Virtual address to physical address conversion
#define mm_v2p(a) ((unsigned long)(a) - KERNEL_VA_OFFSET)
#define mm_p2v(a) ((a) + KERNEL_VA_OFFSET)

#ifndef __ASSEMBLER__
typedef uint32_t pde_t;
typedef uint32_t pte_t;

pte_t init_pagedir[PAGE_DIR_SIZE];

extern void page_directory_load(unsigned long);
extern void enable_paging();
extern void cr4_disable_page_size();

struct mem_map {
    unsigned long pa_start;
    unsigned long va_start;
    unsigned long va_end;
    unsigned long flags;
};

#endif // __ASSEMBLER__

#endif // _ARCH_I386_MM_H 
