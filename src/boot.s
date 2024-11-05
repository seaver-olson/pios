// AArch64 mode
.include "irq.s"

.section ".kernel-header"

// Kernel Header
// See https://www.kernel.org/doc/Documentation/arm64/booting.txt
b _start           /* CODE0 : Executable code */
.word 0            /* CODE1 : Executable code */
.dword _start      /* text_offset : Image load offset, little endian */
.dword 0x0         /* image_size : Image load offset, little endian */
.dword 0x2         /* flags */
.dword 0x0         /* reserved */
.dword 0x0         /* reserved */
.dword 0x0         /* reserved */
.dword 0x644d5241  /* magic */
.dword 0x0         /* reserved */

// To keep this in the first portion of the binary.
.section ".text.boot"

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

    /* Beginning of EL1 setup */

    // set stack before our code
    ldr     x1, =_start

    // set up EL1
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

    // set up exception handlers
    // Uncomment this stuff once you set up the vector table
    // ldr     x2, =_vectors
     //msr     vbar_el1, x2
     //msr     vbar_el2, x2

    // change execution level to EL1
    mov     x2, #0x3c4 // Change execution level to EL1
    // mov     x2, #0x3c0   // Change execution level to EL0
    msr     spsr_el2, x2
    adr     x2, 5f  /* Put the address of the instruction at label 5 (below) into register X2 */
    msr     elr_el2, x2
    eret

5:  mov     sp, x1

    // CALL kernel_main after this...

    // clear bss
    ldr     x5, =__bss_start
    ldr     w6, =__bss_size
3:  cbz     w6, 4f
    str     xzr, [x5], #8
    sub     w6, w6, #1
    cbnz    w6, 3b

    // jump to C code, should not return
4:  bl      kernel_main
    // for failsafe, halt this core too
    // b 1b

.align 11
vectors:
  ventry  sync_invalid_el1t                       // Synchronous EL1t
  ventry  irq_invalid_el1t                        // IRQ EL1t
  ventry  fiq_invalid_el1t                        // FIQ EL1t
  ventry  error_invalid_el1t                      // Error EL1t

  ventry  sync_invalid_el1h                       // Synchronous EL1h
  ventry  el1_irq                                 // IRQ EL1h
  ventry  fiq_invalid_el1h                        // FIQ EL1h
  ventry  error_invalid_el1h                      // Error EL1h

  ventry  sync_invalid_el0_64                     // Synchronous 64-bit EL0
  ventry  irq_invalid_el0_64                      // IRQ 64-bit EL0
  ventry  fiq_invalid_el0_64                      // FIQ 64-bit EL0
  ventry  error_invalid_el0_64                    // Error 64-bit EL0

  ventry  sync_invalid_el0_32                     // Synchronous 32-bit EL0
  ventry  irq_invalid_el0_32                      // IRQ 32-bit EL0
  ventry  fiq_invalid_el0_32                      // FIQ 32-bit EL0
  ventry  error_invalid_el0_32                    // Error 32-bit EL0

.align 2
sync_invalid_el1t:
  handle_invalid_entry  0

irq_invalid_el1t:
  handle_invalid_entry  1

fiq_invalid_el1t:
  handle_invalid_entry  2

error_invalid_el1t:
  handle_invalid_entry  3

sync_invalid_el1h:
  handle_invalid_entry  4

el1_irq:
  kernel_entry
  ldr     x0, handle_irq
  blr     x0
  kernel_exit

fiq_invalid_el1h:
  handle_invalid_entry  6

error_invalid_el1h:
  handle_invalid_entry  7

sync_invalid_el0_64:
  handle_invalid_entry  8

irq_invalid_el0_64:
  handle_invalid_entry  9

fiq_invalid_el0_64:
  handle_invalid_entry  10

error_invalid_el0_64:
  handle_invalid_entry  11

sync_invalid_el0_32:
  handle_invalid_entry  12

irq_invalid_el0_32:
  handle_invalid_entry  13

fiq_invalid_el0_32:
  handle_invalid_entry  14

error_invalid_el0_32:
  handle_invalid_entry  15