#include "rprintf.h"

#define MU_IO_REG 0x3F215040

volatile int *muio_ptr = (volatile int *)(MU_IO_REG);

void putc(int data){
	if (muio_ptr != NULL){
		*muio_ptr = data;
	}
        while (muio_ptr && (1 << 15) == 0);
	*muio_ptr = data;
	//reverse would be data = *muio_ptr; to read the value at muio
}

int main(){
	esp_printf((int (*)(int))putc, "Current Execution Level is %d\r\n", getEL());
	return 0;
}
