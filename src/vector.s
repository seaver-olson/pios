// AArch64 Vector Table for Baremetal OS
//generated by chatgpt plz dont sue me prof, I LITERALLY CANT FIND ANY INFO ON MAKING VECTOR TABLES
//I WILL MAKE THIS MY OWN LATER
.section ".text.boot"
.globl _start
_start:

    // Set up the vector table for exception handling in EL1
    ldr x0, =_vectors
    msr vbar_el1, x0

    // Proceed with other kernel initialization code
    // ...

    // Call kernel_main after setting up the exception vector table
    bl kernel_main

    // Halt the CPU in case of failure
    b .

// Define vector table for EL1
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

// Exception Handlers
// These handlers should be customized to handle different exceptions or simply halt the CPU

// Synchronous exception handler for EL1h
sync_handler_el1h:
    // Handle synchronous exception (e.g., undefined instruction)
    // Log the exception or handle it
    mrs x0, esr_el1  // Get the exception syndrome register
    mrs x1, elr_el1  // Get the exception link register (address where exception occurred)
    // You could log or print these values to a debug console
    b .

// IRQ handler for EL1h
irq_handler_el1h:
    // Handle hardware interrupt (IRQ)
    // Call a C function or handle the interrupt in assembly
    b .

// FIQ handler for EL1h
fiq_handler_el1h:
    // Handle fast interrupt (FIQ)
    b .

// SError handler for EL1h
serror_handler_el1h:
    // Handle system error (SError)
    b .

// Synchronous exception handler for EL1t
sync_handler_el1t:
    // Handle synchronous exception at EL1 with SP_EL0
    b .

// IRQ handler for EL1t
irq_handler_el1t:
    // Handle IRQ at EL1 with SP_EL0
    b .

// FIQ handler for EL1t
fiq_handler_el1t:
    // Handle FIQ at EL1 with SP_EL0
    b .

// SError handler for EL1t
serror_handler_el1t:
    // Handle system error at EL1 with SP_EL0
    b .

// Synchronous exception handler for EL0
sync_handler_el0:
    // Handle synchronous exception taken from EL0
    b .

// IRQ handler for EL0
irq_handler_el0:
    // Handle IRQ taken from EL0
    b .

// FIQ handler for EL0
fiq_handler_el0:
    // Handle FIQ taken from EL0
    b .

// SError handler for EL0
serror_handler_el0:
    // Handle system error taken from EL0
    b .