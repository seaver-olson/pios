#include "rprintf.h"
#include "page.h"
#include "serial.h"
#include "mmu.h"
#include "delays.h"

extern unsigned char __bss_start;
extern unsigned char __bss_end;

void clear_bss(){
	unsigned char *begin_bss = &__bss_start;
	unsigned char *end_bss = &__bss_end;
	int i = 0;
	while (begin_bss != end_bss){
		begin_bss[i] = 0;
		i++;
	}
}

void hexdump(char *buffer, unsigned int length){
	for (int i = 0; i < length; i += 16) {
		esp_printf(putc, "(%x) : ", buffer);
		for (int j=0; j < 16;j++){
			if (j+i>length){
				esp_printf(putc ,"  ");
			} else {
				esp_printf("%02x ", buffer[i + j]);
			}
		}
		esp_printf(putc, "\n");	
	}
	esp_printf(putc, "\n");		 	
}


void kernel_main() {
	clear_bss();

}
