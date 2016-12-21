#include <stdint.h>
#include <arch/i386/mm.h>

void mm_init()
{
    // 1. Discover RAM from multiboot info
    // For now assume that we have 100 MiB of RAM

    // 2. Initialize page frame allocator (free list)

    // 3. Set up kernel page directory
    //   3.1. Get page frame for the kernel page directory
    //   3.2. Setup mappings
    //      Identity map 0x100000-(0x100000+100MiB)
    //   3.3. Load kernel page directory to CR3
}
