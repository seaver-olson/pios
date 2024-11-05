#include "rprintf.h"

void hexdump(char *buffer, unsigned int length){
	for (int i = 0; i < length; i += 16) {
		esp_printf(putc,"(%x) : ", &buffer);
		for (int j=0; j < 16;j++){
			if (j+i>length){
				esp_printf(putc,"  ");
			} else {
				esp_printf(putc,"%02x ", buffer[i + j]);
			}
		}
		esp_printf(putc,"\n");	
	}
	esp_printf(putc,"\n");	 	
}


/*int main(){
	TEST
	char test[] = {'A','B','C','D','E','F','G','H'};
	hexdump(test,16);
	hexdump(main,32);

	return 0;
}
*/
