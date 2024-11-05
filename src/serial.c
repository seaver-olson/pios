#include "serial.h"

#define MU_IO_REG 0x3F215040

volatile int *muio_ptr = (volatile int *)(MU_IO_REG);

void putc(int data){
	if (muio_ptr != NULL){
		*muio_ptr = data;
	}
}

int getc(){
	if (muio_ptr != NULL){
		return *muio_ptr;
	}
	return -1;
}
