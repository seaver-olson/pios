#include "rprintf.h"
#include "serial.h"
#include "color.h"

void red(){
	esp_printf(putc, RED);
}

void green(){
	esp_printf(putc, GREEN);
}

void yellow(){
	esp_printf(putc, YELLOW);
}

void blue(){
	esp_printf(putc, BLUE);
}

void cyan(){
	esp_printf(putc, CYAN);
}

void resetColor(){
	esp_printf(putc, CLEAR);
}

void purple(){
	esp_printf(putc, PURPLE);
}

void black(){
	esp_printf(putc, BLACK);
}

void white(){
	esp_printf(putc, WHITE);
}

