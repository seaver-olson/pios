#include "mailbox.h"

volatile unsigned int  __attribute__((aligned(16))) mailbox[36];

int mailbox_call(unsigned char ch)
{
    unsigned int r = (((unsigned int)((unsigned long)&mailbox)&~0xF) | (ch&0xF));
    do{asm volatile("nop");}while(*MAILBOX_STATUS & MAILBOX_FULL);
    *MAILBOX_WRITE = r;
    while(1) {
        do{asm volatile("nop");}while(*MAILBOX_STATUS & MAILBOX_EMPTY);
        if(r == *MAILBOX_READ)
            return mailbox[1]==MAILBOX_RESPONSE;
    }
    return 0;
}
