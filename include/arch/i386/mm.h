#ifndef _ARCH_I386_MM_H
#define _ARCH_I386_MM_H

void mm_init();

extern void page_directory_load_cr3(unsigned long);
extern void enable_paging();

#endif // _ARCH_I386_MM_H 
