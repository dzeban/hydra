#include <stdint.h>
#include <arch/i386/mm.h>

uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t first_page_table[1024] __attribute__((aligned(4096)));

#define PDE_PRESENT_FLAG     0x00000001
#define PDE_NOT_PRESENT_MASK ~(PDE_PRESENT)

#define PDE_RW_FLAG     0x00000002
#define PDE_RO_MASK     ~(PDE_RW)

#define PDE_USER_FLAG   0x00000004
#define PDE_SUPERVISOR  ~(PDE_USER)

void mm_init()
{
	// Init page directory with RW, NOT_PRESENT entries
	uint32_t init_pde = PDE_RW_FLAG;
	for (int i = 0; i < 1024; i++) {
		page_directory[i] = init_pde;
	}

	// Init first page table with RW, PRESENT entries
	// Map pages to first 1024 frames (0x0 to 4MiB address range)
	uint32_t init_pte = PDE_RW_FLAG | PDE_PRESENT_FLAG;
	for (int i = 0; i < 1024; i++) {
		first_page_table[i] = (i * 0x1000) | init_pte;
	}

	// Set first PDE entry to first_page_table
	uint32_t pde = page_directory[0];
	pde |= (unsigned long)first_page_table;
	pde |= PDE_PRESENT_FLAG;
	page_directory[0] = pde;

	page_directory_load_cr3((unsigned long)page_directory);
	enable_paging();
}
