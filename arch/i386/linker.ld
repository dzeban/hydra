
ENTRY(_start)

/**
 * This is a higher half kernel.
 *
 * Higher half kernel uses virtual addresses relative to "."
 * which is set to 0xc0100000. But bootloader operates in physical addresses,
 * hence it's invalid high address for it. So, we load kernel at physical
 * address 0x100000 using "AT()" section attribute.
 *
 * First things in `_start` entry point is to setup paging to allow higher half
 * kernel works. It will create temp page table of **4MiB pages** with 2 entries:
 *
 * 1. Item #0 - Identity mapping first 4MiB of virtual address space to the first
 *    4MiB of physical address space
 * 2. Item # 768 = 0xc0100000 >> 22 - Identity mapping of 4MiB that includes
 *    kernel virtual address.
 *
 * The reason why we use 4MiB pages (instead of usual 4KiB) is to avoid 2 level
 * page tables and have just a single page table. It just makes initialization
 * simpler and besides this page table is temporary, it will be destroyed soon.
 */
__KERNEL_PA_START  = 0x100000;
__KERNEL_VA_OFFSET = 0xc0000000;
__KERNEL_VA_START  = __KERNEL_PA_START + __KERNEL_VA_OFFSET;

SECTIONS
{
    . = __KERNEL_VA_START;

    /* Start putting section starting at 0x100000 physical address */
    .text ALIGN(4K) : AT(ADDR(.text) - __KERNEL_VA_OFFSET)
    {
        /**
         * First put the multiboot header, as it is required to be put very early
         * early in the image or the bootloader won't recognize the file format.
         * Next we'll put the .text section.
         */
        *(.multiboot)
        *(.text)
    }

    /* Read-only data. */
    .rodata ALIGN(4K) : AT(ADDR(.rodata) - __KERNEL_VA_OFFSET)
    {
        *(.rodata)
    }

    /* Read-write data (initialized) */
    .data ALIGN(4K) : AT(ADDR(.data) - __KERNEL_VA_OFFSET)
    {
        *(.data)
    }

    /* Read-write data (uninitialized) and stack */
    .bss ALIGN(4K) : AT(ADDR(.bss) - __KERNEL_VA_OFFSET)
    {
        *(COMMON)
        *(.bss)
        *(.bootstrap_stack)
    }

    /**
     * Save first free address after kernel sections to __KERNEL_VA_END.
     * We will use it in page frame allocator.  
     */
    . = ALIGN(4K);
    __KERNEL_VA_END = . ;

    /* The compiler may produce other sections, put them in the proper place in
       in this file, if you'd like to include them in the final kernel. */
}
