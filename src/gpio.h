#define MMIO_BASE       0x3F000000

#define GPFSEL0         ((volatile unsigned int*)(MMIO_BASE+0x00200000))
#define GPFSEL1         ((volatile unsigned int*)(MMIO_BASE+0x00200004))
#define GPFSEL2         ((volatile unsigned int*)(MMIO_BASE+0x00200008))
#define GPFSEL3         ((volatile unsigned int*)(MMIO_BASE+0x0020000C))
#define GPFSEL4         ((volatile unsigned int*)(MMIO_BASE+0x00200010))
#define GPFSEL5         ((volatile unsigned int*)(MMIO_BASE+0x00200014))
#define GPSET0          ((volatile unsigned int*)(MMIO_BASE+0x0020001C))
#define GPSET1          ((volatile unsigned int*)(MMIO_BASE+0x00200020))
#define GPCLR0          ((volatile unsigned int*)(MMIO_BASE+0x00200028))
#define GPLEV0          ((volatile unsigned int*)(MMIO_BASE+0x00200034))
#define GPLEV1          ((volatile unsigned int*)(MMIO_BASE+0x00200038))
#define GPEDS0          ((volatile unsigned int*)(MMIO_BASE+0x00200040))
#define GPEDS1          ((volatile unsigned int*)(MMIO_BASE+0x00200044))
#define GPHEN0          ((volatile unsigned int*)(MMIO_BASE+0x00200064))
#define GPHEN1          ((volatile unsigned int*)(MMIO_BASE+0x00200068))
#define GPPUD           ((volatile unsigned int*)(MMIO_BASE+0x00200094))
#define GPPUDCLK0       ((volatile unsigned int*)(MMIO_BASE+0x00200098))
#define GPPUDCLK1       ((volatile unsigned int*)(MMIO_BASE+0x0020009C))

// interrupt registers

#define IRQ_BASIC       ((volatile unsigned int*)(MMIO_BASE+0x0000B200))
#define IRQ_PEND1       ((volatile unsigned int*)(MMIO_BASE+0x0000B204))
#define IRQ_PEND2       ((volatile unsigned int*)(MMIO_BASE+0x0000B208))
#define IRQ_FIQ         ((volatile unsigned int*)(MMIO_BASE+0x0000B20C))
#define IRQ_ENABLE1     ((volatile unsigned int*)(MMIO_BASE+0x0000B210))
#define IRQ_ENABLE2     ((volatile unsigned int*)(MMIO_BASE+0x0000B214))
#define IRQ_ENABLE_BASIC ((volatile unsigned int*)(MMIO_BASE+0x0000B218))
#define IRQ_DISABLE1    ((volatile unsigned int*)(MMIO_BASE+0x0000B21C))
#define IRQ_DISABLE2    ((volatile unsigned int*)(MMIO_BASE+0x0000B220))
#define IRQ_DISABLE_BASIC ((volatile unsigned int*)(MMIO_BASE+0x0000B224))

#define PERIPHERAL_BASE 0x3F000000

#define PERIPHERAL_LENGTH 0x01000000


#define SYSTEM_TIMER_OFFSET 0x3000
#define INTERRUPTS_OFFSET 0xB000
#define MAILBOX_OFFSET 0xB880
#define UART0_OFFSET 0x201000
#define GPIO_OFFSET 0x200000
#define EMMC_OFFSET 0x300000

#define SYSTEM_TIMER_BASE (PERIPHERAL_BASE + SYSTEM_TIMER_OFFSET)
