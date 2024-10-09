#include "rprintf.h"
#include "page.h"
#include "serial.h"

#define NULL (void*) 0

extern unsigned char __bss_start;
extern unsigned char __bss_end;

//edit later to check if time has reached max and if it has figure out how to keep counting
unsigned long get_timer_count(){
	unsigned long *timer_count_register = 0x3f003004;
	return *timer_count_register;
}

void wait(int microseconds){
	unsigned long start_time = get_timer_count();
	unsigned long time = 0;
	while(microseconds > time){
		time = get_timer_count() - start_time;	
	}
}


void hexdump(char *buffer, unsigned int length){
	for (int i = 0; i < length; i += 16) {
		esp_printf("(%x) : ", &buffer);
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
	char *begin_bss = &__bss_start;
	char *end_bss = &__bss_end;
	int i = 0;
	while (begin_bss != end_bss){
		begin_bss[i] = 0;
		i++;
	}
	while(1){
    		hexdump(begin_bss,32);
	}
*/
}
