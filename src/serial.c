#include "rprintf.h"

#define MU_IO_REG     (*(volatile int *)0x3F215040)

void putc(int data){
	MU_IO_REG = data;
}

int getEL(){
	return 1;
}
int main(){
	esp_printf((int (*)(int))putc, "Current Execution Level is %d\r\n", getEL());
	return 0;
}
