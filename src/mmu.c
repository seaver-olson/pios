#include "mmu.h"

// Define L2 tables for different memory regions if necessary
struct table_descriptor_stage1 L1table[512] __attribute__((aligned(4096)));
struct page_descriptor_stage1 L2table0[512] __attribute__((aligned(4096)));  // First 1GB
// Additional L2 tables would go here if you want to map more than 1GB

void mapPages(void *vaddr, void *paddr) {
    unsigned long L2tableIndex = ((unsigned long)vaddr >> 21) & 0x1ff;
    unsigned long L1tableIndex = ((unsigned long)vaddr >> 30) & 0x1ff;
    if (vaddr==0x0 || vaddr == 0x3FE00000){
 	esp_printf(putc,"Mapping: vaddr=%lx, paddr=%lx, L1index=%lx, L2index=%lx\n", (unsigned long)vaddr, (unsigned long)paddr, L1tableIndex, L2tableIndex);
}    
// Set L1 entry to point to the correct L2 table
    L1table[L1tableIndex].type = 3;  // Point to next level (L2 table)
    L1table[L1tableIndex].next_lvl_table = ((unsigned long)&L2table0[0]) >> 12;

    // Configure L2 entry for normal memory
    L2table0[L2tableIndex].attrindx = 0;  // AttrIdx=0 for normal memory
    L2table0[L2tableIndex].type = 1;      // Page descriptor
    L2table0[L2tableIndex].sh = 3;        // Inner sharable
    L2table0[L2tableIndex].ap = 0;        // Kernel RW
    L2table0[L2tableIndex].af = 1;        // Access flag
    L2table0[L2tableIndex].output_addr = (unsigned long)paddr >> 12;
}

int loadPageTable(struct table_descriptor_stage1 *L1table) {

    unsigned long b, r;
    // Enable paging in system regs. This code should be moved elsewhere. Testing only for now.
    // check for 4k granule and at least 36 bits physical address bus */
    asm volatile ("mrs %0, id_aa64mmfr0_el1" : "=r" (r));
    b=r&0xF;
    if(r&(0xF<<28)/*4k*/ || b<1/*36 bits*/) {
        fail("ERROR: 4k granule or 36 bit address space not supported\n");
        return -1;
    }
    // first, set Memory Attributes array, indexed by PT_MEM, PT_DEV, PT_NC in our example
    r=  (0xFF << 0) |    // AttrIdx=0: normal, IWBWA, OWBWA, NTR
        (0x04 << 8) |    // AttrIdx=1: device, nGnRE (must be OSH too)
        (0x44 <<16);     // AttrIdx=2: non cacheable
    asm volatile ("msr mair_el1, %0" : : "r" (r));

    // next, specify mapping characteristics in translate control register
    r=  (0b00LL << 37) | // TBI=0, no tagging
        (b << 32) |      // IPS=autodetected
        (0b10LL << 30) | // TG1=4k
        (0b11LL << 28) | // SH1=3 inner
        (0b01LL << 26) | // ORGN1=1 write back
        (0b01LL << 24) | // IRGN1=1 write back
        (0b0LL  << 23) | // EPD1 enable higher half
        (25LL   << 16) | // T1SZ=25, 3 levels (512G)
        (0b00LL << 14) | // TG0=4k
        (0b11LL << 12) | // SH0=3 inner
        (0b01LL << 10) | // ORGN0=1 write back
        (0b01LL << 8) |  // IRGN0=1 write back
        (0b0LL  << 7) |  // EPD0 enable lower half
        (25LL   << 0);   // T0SZ=25, 3 levels (512G)
    asm volatile ("msr tcr_el1, %0; isb" : : "r" (r));
    

    // tell the MMU where our translation tables are. TTBR_CNP bit not documented, but required
    // lower half, user space
    asm volatile ("msr ttbr0_el1, %0" : : "r" ((unsigned long)L1table));

    // finally, toggle some bits in system control register to enable page translation
    asm volatile ("dsb ish; isb; mrs %0, sctlr_el1" : "=r" (r));
    r|=0xC00800;     // set mandatory reserved bits
    r&=~((1<<25) |   // clear EE, little endian translation tables
         (1<<24) |   // clear E0E
         (1<<19) |   // clear WXN
         (1<<12) |   // clear I, no instruction cache
         (1<<4) |    // clear SA0
         (1<<3) |    // clear SA
         (1<<2) |    // clear C, no cache at all
         (1<<1));    // clear A, no aligment check
    r|=  (1<<0);     // set M, enable MMU
    asm volatile ("msr sctlr_el1, %0; isb" : : "r" (r));
    return 0;
}

void setupIdentityMap() {
    void *vaddr = (void *)0x0;
    void *paddr = (void *)0x0;

    // Map first 1GB with identity mapping
    green();
    esp_printf(putc,"Setting Identity map... \n");
    esp_printf(putc,"--------------------------------------------------\n");
    for (int i = 0; i < 512; i++) {
        mapPages(vaddr, paddr);
	if (i ==1)esp_printf(putc, "...\n");
        vaddr += 0x200000;  // Increment by 2MB
        paddr += 0x200000;  // Increment by 2MB
    }
    esp_printf(putc,"--------------------------------------------------\n");
    resetColor();
    // Enable MMU with the loaded page tables
    if (loadPageTable(L1table) != 0){
	    fail("ERROR: MMU FAILED TO BE INITIALIZED\n");
    	return;
	}
    green();
    esp_printf(putc, "MMU INITIALIZED\n");
}
