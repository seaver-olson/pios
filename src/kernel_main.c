#include <stdio.h>

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
		printf("(%x) : ", &buffer);
		for (int j=0; j < 16;j++){
			if (j+i>length){
				printf("  ");
			} else {
				printf("%02x ", buffer[i + j]);
			}
		}
		printf("\n");	
	}
	printf("\n");	 	
}


void kernel_main() {
	char *begin_bss = &__bss_start;
	char *end_bss = &__bss_end;
	char *currentCell = begin_bss;
	//clear bss buffer	
	while(&currentCell != &end_bss){
		*currentCell = '0';
		currentCell = currentCell++;
	} 
	while(1){
    		hexdump(begin_bss,32);
	}
}
