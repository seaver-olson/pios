#include "usb.h"
#include "delay.h"
#include "rprintf.h"
#include "serial.h"


void usb_init(){
	*((volatile uint32_t *)USB_POWER_REGISTER) = 0x01;
	*((volatile uint32_t *)USB_RESET_REGISTER) = 0x01;
	delay(50);//for reset delay
	*((volatile uint32_t *)USB_RESET_REGISTER) = 0x00;
	//enable_usb_interrupts();//later create a function to allow interrupts
	uint32_t status = *((volatile uint32_t *)USB_STATUS_REGISTER);
    	if (status != 0x0) {
		red();
		esp_printf(putc, "ERROR: USB CONTROLLER FAILED TO BE INITIALIZED");
		return;
	}
	green();
	esp_printf(putc, "USB CONTROLLER INITIALIZED");
	resetColor();
}

int usb_enumerate_device(){
	struct usb_device_descriptor descriptor;
	
	//usb_ctrl_tsfr(USB_REQUEST_GET_DESCRIPTOR
	return 0;
}
/*
void enable_usb_interrupts(){

}
*/



