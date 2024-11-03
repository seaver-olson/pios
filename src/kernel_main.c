#include "rprintf.h"
#include "page.h"
#include "serial.h"
#include "mmu.h"
#include "delays.h"
#include "fat.h"

char glbl[128];

void clear_bss(){
	extern unsigned char __bss_end;
	extern unsigned char __bss_start;
	unsigned char *begin_bss = &__bss_start;
	unsigned char *end_bss = &__bss_end;
	int i = 0;
	while (begin_bss != end_bss){
		begin_bss[i] = 0;
		i++;
	}
}

unsigned int getEL(){
  unsigned int el;
  asm("mrs %0,CurrentEL"
      : "=r"(el)
      :
      :);

  return el;
}

void hexdump(char *buffer, unsigned int length){
	for (int i = 0; i < length; i += 16) {
		esp_printf(putc, "(%x) : ", buffer);
		for (int j=0; j < 16;j++){
			if (j+i>length){
				esp_printf(putc ,"  ");
			} else {
				esp_printf(putc, "%02x ", buffer[i + j]);
			}
		}
		esp_printf(putc, "\n");	
	}
	esp_printf(putc, "\n");		 	
}


void kernel_main() {
	//clear_bss();
	if (fatInit() != 0){
		esp_printf(putc, "[ERROR] FAT INIT FAILED");
		return ;
	}
	esp_printf(putc, "FAT SYS INITIALIZED\n");

	while (1){

	}
}
