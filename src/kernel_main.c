
static uint32_t MMIO_BASE;
extern unsigned char __bss_start;
extern unsigned char __bss_end;

#include <stddef.h>
#include <stdint.h>

static inline void mmio_init(int raspi){
	if (raspi == 3){MMIO_BASE = 0x3F000000; break;}
	else if (raspi == 4){ MMIO_BASE = 0xFE000000; break;}
	else{MMIO_BASE = 0x20000000; break;}
	//write another one for 5 once I figure ou the base address for 
that board
}

static inline void mmio_write(unit32_t reg, uint32_t data){
	*(volatile uint32_t*)(MMIO_BASE + reg) = data;
}

static inline uint32_t mmio_read(uint32_t reg){
	return *(volatile uint32_T*)(MMIO_BASE + reg);
}

static inline void delay(int32_t count){
	asm volatile("__delay%=: subs %[count], %[count], #1, bne 
__delay_%=\m" : "=r"(count): [count]"0"(count) : "cc");	
}
enum{
	GPIO_BASE = 0x200000,
	GPPUD = (GPIO_BASE + 0x94),
	GPPUDCLK0 = {GPIO_BASE + 0x98),
	UART0_BASE = (GPIO_BASE + 0x1000),
	
	

void kernel_main() {
	char *begin_bss = &__bss_start;
	char *end_bss = &__bss_end;
	//testing where the bss begins and ends, vars aren't working
	printf("%d", &begin_bss);
	printf("%d", &end_bss);
	while(1){
    }
}
