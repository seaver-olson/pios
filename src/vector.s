// Define vector table for EL1, handling synchronous exceptions, IRQ, FIQ, and SError
.section ".vectors"
.globl _vectors
_vectors:

    // EL1h synchronous exception handler (when running at EL1 with SP_EL1)
    b sync_handler_el1h
    
    // EL1h IRQ exception handler
    b irq_handler_el1h
    
    // EL1h FIQ exception handler
    b fiq_handler_el1h
    
    // EL1h SError exception handler
    b serror_handler_el1h

    // EL1t synchronous exception handler (when running at EL1 with SP_EL0)
    b sync_handler_el1t
    
    // EL1t IRQ exception handler
    b irq_handler_el1t
    
    // EL1t FIQ exception handler
    b fiq_handler_el1t
    
    // EL1t SError exception handler
    b serror_handler_el1t

    // EL0 synchronous exception handler
    b sync_handler_el0
    
    // EL0 IRQ exception handler
    b irq_handler_el0
    
    // EL0 FIQ exception handler
    b fiq_handler_el0
    
    // EL0 SError exception handler
    b serror_handler_el0
