// AArch64 mode

.section ".kernel-header"

// Kernel Header
// See https://www.kernel.org/doc/Documentation/arm64/booting.txt
b _start           /* CODE0 : Executable code */
.word 0           /* CODE1 : Executable code */
.dword _start    /* text_offset : Image load offset, little endian */
.dword 0x0        /* image_size : Image load offset, little endian */
.dword 0x2        /* flags */
.dword 0x0        /* reserved */
.dword 0x0        /* reserved */
.dword 0x0        /* reserved */
.dword 0x644d5241 /* magic */
.dword 0x0        /* reserved */

// To keep this in the first portion of the binary.
.section ".text.boot"

// Make _start global.
.globl _start

// Entry point for the kernel. Registers:
// x0 -> 32 bit pointer to DTB in memory (primary core only) / 0 (secondary cores)
// x1 -> 0
// x2 -> 0
// x3 -> 0
// x4 -> 32 bit kernel entry point, _start location
_start:
    mrs x1, mpidr_el1
    and x1, x1, #3
    cbz x1, maincore

notmaincore: // CPU id > 0: stop
    wfi
    b notmaincore

maincore:
    // set stack before our code
    ldr     x5, =_start
    mov     sp, x5

    // clear bss
    ldr     x5, =__bss_start
    ldr     w6, =__bss_size
3:  cbz     w6, 4f
    str     xzr, [x5], #8
    sub     w6, w6, #1
    cbnz    w6, 3b

    // Set up EL1
    ldr     x1, =_start

    // Current exception level
    mrs     x0, CurrentEL
    and     x0, x0, #12 // clear reserved bits

    // running at EL3?
    cmp     x0, #12
    bne     5f
    // should never be executed, just for completeness
    mov     x2, #0x5b1
    msr     scr_el3, x2
    mov     x2, #0x3c9
    msr     spsr_el3, x2
    adr     x2, 5f
    msr     elr_el3, x2
    eret

    // running at EL2?
5:  cmp     x0, #4
    beq     5f
    msr     sp_el1, x1
    msr     sp_el0, x1

    // enable CNTP for EL1
    mrs     x0, cnthctl_el2
    orr     x0, x0, #3
    msr     cnthctl_el2, x0
    msr     cntvoff_el2, xzr

    // enable AArch64 in EL1
    mov     x0, #(1 << 31)      // AArch64
    orr     x0, x0, #(1 << 1)   // SWIO hardwired on Pi3
    msr     hcr_el2, x0
    mrs     x0, hcr_el2

    // Set up exception handlers
    ldr     x2, =vectors
    msr     vbar_el1, x2
    msr     vbar_el2, x2

    // Change execution level to EL1
    mov     x2, #0x3c4 // Change execution level to EL1
    msr     spsr_el2, x2
    adr     x2, 5f
    msr     elr_el2, x2
    eret

5:  mov     sp, x1

// Jump to C code, should not return
4:  bl      kernel_main
    // For failsafe, halt this core too
    b .

// Exception vector table and handlers
.align 11
vectors:
    b vector_el1_sync      // Synchronous exception from EL1
    b vector_el1_irq       // IRQ exception from EL1
    b vector_el1_fiq       // FIQ exception from EL1
    b vector_el1_serror    // SError exception from EL1
    b vector_el0_sync      // Synchronous exception from EL0
    b vector_el0_irq       // IRQ exception from EL0
    b vector_el0_fiq       // FIQ exception from EL0
    b vector_el0_serror     // SError exception from EL0

// EL1 synchronous exception handler
vector_el1_sync:
    b . // Placeholder: Handle EL1 sync exception

// EL1 IRQ exception handler
vector_el1_irq:
    b . // Placeholder: Handle EL1 IRQ exception

// EL1 FIQ exception handler
vector_el1_fiq:
    b . // Placeholder: Handle EL1 FIQ exception

// EL1 SError exception handler
vector_el1_serror:
    b . // Placeholder: Handle EL1 SError exception

// EL0 synchronous exception handler
vector_el0_sync:
    b . // Placeholder: Handle EL0 sync exception

// EL0 IRQ exception handler
vector_el0_irq:
    b . // Placeholder: Handle EL0 IRQ exception

// EL0 FIQ exception handler
vector_el0_fiq:
    b . // Placeholder: Handle EL0 FIQ exception

// EL0 SError exception handler
vector_el0_serror:
    b . // Placeholder: Handle EL0 SError exception
