#include "gpio.h"

extern volatile unsigned int mailbox[36];

#define MAILBOX_REQUEST    0

#define MAILBOX_CH_POWER   0
#define MAILBOX_CH_FB      1
#define MAILBOX_CH_VUART   2
#define MAILBOX_CH_VCHIQ   3
#define MAILBOX_CH_LEDS    4
#define MAILBOX_CH_BTNS    5
#define MAILBOX_CH_TOUCH   6
#define MAILBOX_CH_COUNT   7
#define MAILBOX_CH_PROP    8

#define MAILBOX_TAG_GETSERIAL      0x10004
#define MAILBOX_TAG_SETCLKRATE     0x38002
#define MAILBOX_TAG_LAST           0

#define VIDEOCORE_MBOX  (MMIO_BASE+0x0000B880)
#define MAILBOX_READ       ((volatile unsigned int*)(VIDEOCORE_MBOX+0x0))
#define MAILBOX_POLL       ((volatile unsigned int*)(VIDEOCORE_MBOX+0x10))
#define MAILBOX_SENDER     ((volatile unsigned int*)(VIDEOCORE_MBOX+0x14))
#define MAILBOX_STATUS     ((volatile unsigned int*)(VIDEOCORE_MBOX+0x18))
#define MAILBOX_CONFIG     ((volatile unsigned int*)(VIDEOCORE_MBOX+0x1C))
#define MAILBOX_WRITE      ((volatile unsigned int*)(VIDEOCORE_MBOX+0x20))
#define MAILBOX_RESPONSE   0x80000000
#define MAILBOX_FULL       0x80000000
#define MAILBOX_EMPTY      0x40000000

int mailbox_call(unsigned char ch);
