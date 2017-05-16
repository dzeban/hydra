#include <multiboot.h>

#include <stdio.h>
#include <kernel/tty.h>
#include <arch/init.h>
#include <cpuid.h>

#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

int boot_info_parse(unsigned long boot_info_addr)
{
    multiboot_info_t *mbi;

    // Set MBI to the address of the Multiboot information structure.
    mbi = (multiboot_info_t *)boot_info_addr;

    // Are mem_* valid?
    if (CHECK_FLAG (mbi->flags, 0)) {
        printf("mem_lower = %uKB, mem_upper = %uKB\n",
                mbi->mem_lower, mbi->mem_upper);
    }

    // Bits 4 and 5 are mutually exclusive!
    if (CHECK_FLAG (mbi->flags, 4) && CHECK_FLAG (mbi->flags, 5)) {
        printf ("Both bits 4 and 5 are set.\n");
        return -1;
    }

    // Are mmap_* valid?
    if (CHECK_FLAG (mbi->flags, 6)) {
        multiboot_mmap_entry_t *mmap;

        printf("mmap_addr = 0x%x, mmap_length = 0x%x\n",
                 mbi->mmap_addr,  mbi->mmap_length);

        mmap = (multiboot_mmap_entry_t *)mbi->mmap_addr;
        while ((unsigned long)mmap < mbi->mmap_addr + mbi->mmap_length) {
            printf("0x%x%x + %x%x(%s)   ",
                    mmap->addr_high, mmap->addr_low,
                    mmap->len_high, mmap->len_low,
                    mmap->type == 1 ? "available" : "reserved");
            mmap = (multiboot_mmap_entry_t *)((unsigned long)mmap + mmap->size + sizeof(mmap->size));
        }
    }

    return 0;
}

void kernel_early(unsigned long magic, unsigned long boot_info_addr)
{
    terminal_initialize();

    /* Am I booted by a Multiboot-compliant boot loader? */
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        printf("Invalid magic number: 0x%x\n", (unsigned)magic);
        return;
    }

    if (boot_info_parse(boot_info_addr)) {
        printf("Invalid boot info\n");
        return;
    }

    arch_initialize();

    printf("Hey!");
}

void kernel_main(void)
{
    printf("Hello\n");
}
