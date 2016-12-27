#ifndef _KERNEL_MM_H
#define _KERNEL_MM_H


// These variables are put by linked
// according to linker.ld
extern unsigned long KERNEL_PA_START;
extern unsigned long KERNEL_PA_DATA;
extern unsigned long KERNEL_PA_END;
extern unsigned long KERNEL_VA_OFFSET;
extern unsigned long KERNEL_VA_START;
extern unsigned long KERNEL_VA_DATA;
extern unsigned long KERNEL_VA_END;

// Get casted address of variable
// Used with KERNEL_PA_* variables
#define ADDR(x) (unsigned long)&x

// Last available physical address.
// Calculated from multiboot provided memory map.
// Used in page frame allocator.
unsigned long MM_LAST_PA;

void mm_init();

struct mm_page_frame {
    int i;
    struct mm_page_frame *next;
};

struct mm_page_frame_allocator {
    int free;
    struct mm_page_frame *list_head;
};

#endif // _KERNEL_MM_H
