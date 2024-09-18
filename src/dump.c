#include <stdio.h>

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


int main(){
	char test[] = {'A','B','C','D','E','F','G','H'};
	hexdump(test,16);
	hexdump(main,32);
	return 0;
}
