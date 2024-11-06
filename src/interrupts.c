#include "interrupts.h"

static interrupt_registers_t * interrupt_regs;
static timer_registers_t * timer_regs;

static interrupt_handler_f handlers[72];
static interrupt_clearer_f clearers[72];


extern void move_exception_vector(void);

int timer_init(void) {
    timer_regs = (timer_registers_t *)SYSTEM_TIMER_BASE;
    register_irq_handler(1, timer_irq_handler, timer_irq_clearer);
    return 0;
}

static void timer_irq_handler(void) {
    printf("timeout :)\n");
    timer_set(3000000);
}

static void timer_irq_clearer(void) {
    timer_regs->control.timer1_matched = 1;
}

void irq_handler(void) {
    printf("IRQ HANDLER\n");
    while(1);
}
__inline__ int INTERRUPTS_ENABLED(void) {
    int res;
    __asm__ __volatile__("mrs %[res], CPSR": [res] "=r" (res)::);
    return ((res >> 7) & 1) == 0;
}
__inline__ void ENABLE_INTERRUPTS(void) {
    if (!INTERRUPTS_ENABLED()) {
        __asm__ __volatile__("cpsie i");
    }
}
void __attribute__ ((interrupt ("ABORT"))) reset_handler(void) {
    printf("RESET HANDLER\n");
    while(1);
}
void __attribute__ ((interrupt ("ABORT"))) prefetch_abort_handler(void) {
    printf("PREFETCH ABORT HANDLER\n");
    while(1);
}
void __attribute__ ((interrupt ("ABORT"))) data_abort_handler(void) {
    printf("DATA ABORT HANDLER\n");
    while(1);
}
void __attribute__ ((interrupt ("UNDEF"))) undefined_instruction_handler(void) {
    printf("UNDEFINED INSTRUCTION HANDLER\n");
    while(1);
}
void __attribute__ ((interrupt ("SWI"))) software_interrupt_handler(void) {
    printf("SWI HANDLER\n");
    while(1);
}
void __attribute__ ((interrupt ("FIQ"))) fast_irq_handler(void) {
    printf("FIQ HANDLER\n");
    while(1);
}

int interrupts_init(void) {
    interrupt_regs = (interrupt_registers_t *)IRQ_BASIC;
    bzero(handlers, sizeof(interrupt_handler_f) * NUM_IRQS);
    bzero(clearers, sizeof(interrupt_clearer_f) * NUM_IRQS);
    interrupt_regs->irq_basic_disable = 0xffffffff; // disable all interrupts
    interrupt_regs->irq_gpu_disable1 = 0xffffffff;
    interrupt_regs->irq_gpu_disable2 = 0xffffffff;
    move_exception_vector();
    ENABLE_INTERRUPTS();
    return 0;
}

void register_irq_handler(irq_number_t irq_num, interrupt_handler_f handler, interrupt_clearer_f clearer) {
    uint32_t irq_pos;
    if (IRQ_IS_BASIC(irq_num)) {
        irq_pos = irq_num - 64;
        handlers[irq_num] = handler;
        clearers[irq_num] = clearer;
        interrupt_regs->irq_basic_enable |= (1 << irq_pos);
    }
    else if (IRQ_IS_GPU2(irq_num)) {
        irq_pos = irq_num - 32;
        handlers[irq_num] = handler;
        clearers[irq_num] = clearer;
        interrupt_regs->irq_gpu_enable2 |= (1 << irq_pos);
    }
    else if (IRQ_IS_GPU1(irq_num)) {
        irq_pos = irq_num;
        handlers[irq_num] = handler;
        clearers[irq_num] = clearer;
        interrupt_regs->irq_gpu_enable1 |= (1 << irq_pos);
    }
}

void irq_handler(void) {
    int j;
    for (j = 0; j < NUM_IRQS; j++) {
        // If the interrupt is pending and there is a handler, run the handler
        if (IRQ_IS_PENDING(interrupt_regs, j)  && (handlers[j] != 0)) {
            clearers[j]();
            ENABLE_INTERRUPTS();
            handlers[j]();
            DISABLE_INTERRUPTS();
            return;
        }
    }
}