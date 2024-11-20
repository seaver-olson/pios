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
	char buffer[512];
	//initialize timer and enable IRQ
	timer_init(1);
	asm("msr DAIFCLr, #2");
	success("timer initialized");

	//set up the page table
	setupIdentityMap();
    
	//initialize the SD card
	if (sd_init()!=SD_OK){
		fail("[ERROR] SD INIT FAILED");
		return;
	}
	success("SD CARD INITIALIZED\n");

	//initialize FAT filesystem
	if (fatInit() != 0){
		fail("[ERROR] FAT INIT FAILED");
		return ;
	}
	success( "FAT SYS INITIALIZED\n");

	//test SD card write and read
	unsigned char writeBuffer[512] = "Test data for SD card write";
	unsigned char readBuffer[512];
	if (sd_writeblock(writeBuffer, 0, 1) != 0){
		fail("[ERROR] SD WRITE FAILED");
		return;
	}
	success("SD CARD WRITE SUCCESSFUL\n");
	if (sd_readblock(0, readBuffer, 1) != 0){
		fail("[ERROR] SD READ FAILED");
		return;
	}
	success("SD CARD READ SUCCESSFUL\n");

	hexdump(readBuffer, 512);


	while (1){
		wait_msec(1000);
		//readLine(buffer);
	}
}
