#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <kernel/mm.h>
#include <kernel/vga.h>

// TODO: configure build system to use just
// #include <arch/mm.h>
#include <arch/i386/mm.h>

struct mm_page_frame_allocator pf_allocator = {
    .free = 0,
    .list_head = NULL,
};

int mm_page_frame_allocator_init1()
{
    // Start from the end of the kernel
    // Round up to the next page frame
    // (Alignment here is not actually necessary because linker already did it)
    char *pfa = (char *)((ADDR(KERNEL_PA_END) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1));
    // printf("pfa %p\n", pfa);

    // We have only first 4MiB page mapped (with init_pagedir),
    // so account 4KiB page frames from
    // KERNEL_PA_START..4MiB range
    unsigned long last = 4 * 0x100000;

    struct mm_page_frame *pf;
    for (int i = 0; (unsigned long)pfa < last; pfa += PAGE_SIZE, i++) {
        pf = (struct mm_page_frame *)pfa;
        pf->i = i;
        pf->next = pf_allocator.list_head;
        // printf("pf[%d] %p %p\n", pf->i, pf, pf->next);

        pf_allocator.list_head = pf;
        pf_allocator.free++;
    }

    return 0;
}

char *mm_page_frame_get()
{
    // Get page frame from the list head
    struct mm_page_frame *pf = pf_allocator.list_head;
    pf_allocator.list_head = pf->next;
    char *pfa = (char *)pf;
    return pfa;
}

void mm_page_frame_put(char *pfa)
{
    // Return page frame to the list head
    struct mm_page_frame *pf;
    pf = (struct mm_page_frame *)pfa;
    pf->i = pf_allocator.free++;
    pf->next = pf_allocator.list_head;
    pf_allocator.list_head = pf;
}

int mm_map_pages_identity(pde_t page_dir[], struct mem_map *map)
{
    pte_t *page_table;
    pde_t pde;

    unsigned long va = map->va_start;
    unsigned long pa = map->pa_start;

    while (va < map->va_end) {

        pde = page_dir[PDX(va)];
        if (!(pde & PDE_PRESENT_FLAG)) {
            page_table = (pte_t *)mm_page_frame_get();
            memset(page_table, 0, PAGE_TABLE_SIZE * sizeof(pte_t));
            // Save new page table in page directory
            pde = (unsigned long)page_table | PDE_PRESENT_FLAG | PDE_RW_FLAG;
            page_dir[PDX(va)] = pde;
        } else {
            page_table = (pte_t *)PTE_ADDR(pde);
        }

        page_table[PTX(va)] = pa | map->flags;

        va += PAGE_SIZE;
        pa += PAGE_SIZE;
    }

    return 0;
}

pde_t *mm_kernel_pagedir_init()
{
    // Declare identity mapping of kernel pages with correct flags
    struct mem_map kernel_map[] = {
        // From 0xc0000000 to kernel start (first 1MiB) maps to the I/O space
        {
            0x0,
            ADDR(KERNEL_VA_OFFSET),
            ADDR(KERNEL_VA_START),
            PTE_PRESENT_FLAG | PTE_RW_FLAG
        },

        // Kernel .text and .rodata - do identity mapping too,
        // but with read-only permissions.
        {
            ADDR(KERNEL_PA_START),
            ADDR(KERNEL_VA_START),
            ADDR(KERNEL_VA_DATA),
            PTE_PRESENT_FLAG
        },

        // Map .data and .bss section with r/w permissions.
        {
            ADDR(KERNEL_PA_DATA),
            ADDR(KERNEL_VA_DATA),
            ADDR(KERNEL_VA_END),
            PTE_PRESENT_FLAG | PTE_RW_FLAG
        },

        // Map the rest of the frames as r/w but not present
        {
            ADDR(KERNEL_PA_END),
            ADDR(KERNEL_VA_END),
            MM_LAST_PA + KERNEL_VA_OFFSET,
            PTE_RW_FLAG
        },
    };

    // Get page frame for the kernel page directory
    pde_t *kernel_pagedir = (pde_t *)mm_page_frame_get();
    memset(kernel_pagedir, 0, PAGE_DIR_SIZE * sizeof(pde_t));

    // Setup mappings
    int kernel_map_size = sizeof(kernel_map)/sizeof(*kernel_map);
    for (int i = 0; i < kernel_map_size; i++) {
        mm_map_pages_identity(kernel_pagedir, &kernel_map[i]);
    }
    return kernel_pagedir;
}

void mm_init()
{
    pde_t *kernel_pagedir;

    // Initialize page frame allocator with
    // page frames from KERNEL_PA_END..4MiB range
    mm_page_frame_allocator_init1();

    // Set up kernel page directory
    kernel_pagedir = mm_kernel_pagedir_init();
    page_directory_load((unsigned long)kernel_pagedir);
    cr4_disable_page_size();
}
