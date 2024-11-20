#include "rprintf.h"
#include "page.h"
#include "mmu.h"
#include "delays.h"
#include "fat.h"
#include "mailbox.h"
#include "systimer.h"

extern unsigned char __bss_end;
extern unsigned char __bss_start;

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
				esp_printf(putc, "%02x ", buffer[i + j]);
			}
		}
		esp_printf(putc, "\n");	
	}
	esp_printf(putc, "\n");
}

unsigned int getEL(){
  unsigned int el;
  asm("mrs %0,CurrentEL"
      : "=r"(el)
      :
      :);

  return el;
}



void kernel_main() {
	//clear_bss();
	//if (UART_MIS & (1 << UART_MIS_RXMIS)) {
	timer_init(1);
	asm("msr DAIFCLr, #2");
	success("timer initialized");
	setupIdentityMap();
        if (sd_init()!=SD_OK){
		fail("[ERROR] SD INIT FAILED");
		return;
	}
	success("SD CARD INITIALIZED\n");
	if (fatInit() != 0){
		fail("[ERROR] FAT INIT FAILED");
		return ;
	}
	success( "FAT SYS INITIALIZED\n");
	while (1){
		wait_msec(1000);
		readLine(buffer);
	}
}
