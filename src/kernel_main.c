#include "rprintf.h"
#include "page.h"
#include "mmu.h"
#include "delays.h"
#include "fat.h"
#include "mailbox.h"


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
	mailbox[0] = 8*4;                  // length of the message
    	mailbox[1] = MAILBOX_REQUEST;         // this is a request message
    
   	mailbox[2] = MAILBOX_TAG_GETSERIAL;   // get serial number command
    	mailbox[3] = 8;                    // buffer size
    	mailbox[4] = 8;
    	mailbox[5] = 0;                    // clear output buffer
    	mailbox[6] = 0;

    	mailbox[7] = MAILBOX_TAG_LAST;

    	if (mailbox_call(MAILBOX_CH_PROP)) {
        	esp_printf(putc, "My serial number is: ");
        	esp_printhex(mailbox[6]);
        	esp_printhex(mailbox[5]);
        	esp_printf(putc, "\n");
    	} else {
        	fail("Unable to query serial!\n");
        }
        setupIdentityMap();
	if (fatInit() != 0){
		fail("[ERROR] FAT INIT FAILED");
		return ;
	}
	success( "FAT SYS INITIALIZED\n");
	while (1){
		wait_msec(1000);
		esp_printf(putc, getc());
	}
}
