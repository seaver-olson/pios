#include "mmu.h"

// Define L2 tables for different memory regions if necessary
struct table_descriptor_stage1 L1table[512] __attribute__((aligned(4096)));
struct page_descriptor_stage1 L2table0[512] __attribute__((aligned(4096)));  // First 1GB
// Additional L2 tables would go here if you want to map more than 1GB

void mapPages(void *vaddr, void *paddr) {
    unsigned long L2tableIndex = ((unsigned long)vaddr >> 21) & 0x1ff;
    unsigned long L1tableIndex = ((unsigned long)vaddr >> 30) & 0x1ff;
    esp_printf(putc,"Mapping: vaddr=%lx, paddr=%lx, L1index=%lx, L2index=%lx\n",
                (unsigned long)vaddr, (unsigned long)paddr, L1tableIndex, L2tableIndex);
    // Set L1 entry to point to the correct L2 table
    L1table[L1tableIndex].type = 3;  // Point to next level (L2 table)
    L1table[L1tableIndex].next_lvl_table = ((unsigned long)&L2table0[0]) >> 12;

    // Configure L2 entry for normal memory
    L2table0[L2tableIndex].attrindx = 0;  // AttrIdx=0 for normal memory
    L2table0[L2tableIndex].type = 1;      // Page descriptor
    L2table0[L2tableIndex].sh = 3;        // Inner sharable
    L2table0[L2tableIndex].ap = 0;        // Kernel RW
    L2table0[L2tableIndex].af = 1;        // Access flag
    L2table0[L2tableIndex].output_addr = (unsigned long)paddr >> 21;
}

int loadPageTable(struct table_descriptor_stage1 *L1table) {
    unsigned long r, b;

    // Check for 4K granule support
    asm volatile ("mrs %0, id_aa64mmfr0_el1" : "=r" (r));
    b = r & 0xF;
    if ((r & (0xF << 28)) || b < 1) {
        return -1;
    }
    // Set MAIR_EL1 for normal, device, and non-cacheable memory
    r = (0xFF << 0) | (0x04 << 8) | (0x44 << 16);
    asm volatile ("msr mair_el1, %0" : : "r" (r));

    // Configure TCR_EL1 with adjusted T0SZ for 1GB address space
    r = (0b00LL << 37) | (b << 32) | (0b10LL << 30) |
        (0b11LL << 28) | (0b01LL << 26) | (0b01LL << 24) |
        (0b0LL << 23) | (34LL << 0);  // T0SZ=34 for 1GB
    asm volatile ("msr tcr_el1, %0; isb" : : "r" (r));

    // Set TTBR0_EL1 to L1 table
    asm volatile ("msr ttbr0_el1, %0" : : "r" ((unsigned long)L1table));
    
    // Enable the MMU in SCTLR_EL1, keeping cache settings minimal
    asm volatile ("dsb ish; isb; mrs %0, sctlr_el1" : "=r" (r));
    r |= 0xC00800;     // Set reserved bits
    r &= ~((1 << 1) | (1 << 3) | (1 << 4) | (1 << 2) | (1 << 12)); // Disable caches initially
    r |= (1 << 0);     // Enable MMU
    asm volatile ("msr sctlr_el1, %0; isb" : : "r" (r));
    esp_printf(putc, "success");
    return 0;
}

void setupIdentityMap() {
    void *vaddr = (void *)0x0;
    void *paddr = (void *)0x0;

    // Map first 1GB with identity mapping
    for (int i = 0; i < 512; i++) {
        mapPages(vaddr, paddr);
        vaddr += 0x200000;  // Increment by 2MB
        paddr += 0x200000;  // Increment by 2MB
    }
    // Enable MMU with the loaded page tables
    if (loadPageTable(L1table) != 0){
	red();
	esp_printf(putc,"ERROR: MMU FAILED TO BE INITIALIZED\n");
    	return;
	}
    green();
    esp_printf(putc, "MMU INITIALIZED\n");
}
