#define USB_BASE_ADDRESS 0x20980000//edit later
#define USB_POWER_REGISTER (USB_BASE_ADDRESS + 0x01)
#define USB_RESET_REGISTER (USB_BASE_ADDRESS + 0x02)

struct usb_device_descriptor {

	int bLength;//Size of this descriptor in bytes
	int bDescriptorType;//Device descirptor type
	int bcdUSB;// USB spec in Binary-coded dec ie 2.10 is 210h
	//possibly remove class sys
	int bDeviceClass;//Class code assigned by the USB-IF
	int bDeviceSubClass;//Subclass code
	int bDeviceProtocol;//protocol code also assigned by the USB-IF

	int bMaxPacketSize();//max packet size for endpoint zero(9,16,32 or 64)
	int idVendor();//vendor ID again assigned by USB-IF
	int idProduct();//product ID ,bet you wont guess who assigns it
	int bcdDevice;//device release number in binary coded decimal 
}

void usb_init();
int usb_enumerate_device();
//void enable_usb_interrupts();
