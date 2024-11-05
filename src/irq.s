.text

.align 2
irq_vector_init:
  adr     x0, vectors                             // load VBAR_EL1 with virtual
  msr     vbar_el1, x0                            // vector table address
  ret


enable_irq:
  msr     daifclr, #2
  ret


disable_irq:
  msr     daifset, #2
  ret


show_invalid_entry_message:
  logregs
  ret


enable_ic_bcm283x:
  mov     w0, #0x00000002
  adrp    x1, 0x3f00b000 + _start
  str     w0, [x1, #0x210]                        // [0x3f00b210] = 0x00000002
  ret


enable_ic_bcm2711:
  mov     w0, #0x00000002
  adrp    x1, 0xfe00b000 + _start
  str     w0, [x1, #0x210]                        // [0xfe00b210] = 0x00000002
  ret


handle_irq_bcm283x:
  stp     x29, x30, [sp, #-16]!                   // Push frame pointer, procedure link register on stack.
  mov     x29, sp                                 // Update frame pointer to new stack location.
  adrp    x1, 0x3f00b000 + _start
  ldr     w0, [x1, #0x204]                        // w0 = [0x3f00b204]
  cmp     w0, #2
  b.ne    1f
  bl      handle_timer_irq
.if UART_DEBUG
  b       2f
.endif
1:
.if UART_DEBUG
  bl      log_unknown_interrupt_value
2:
.endif
  ldp     x29, x30, [sp], #16                     // Pop frame pointer, procedure link register off stack.
  ret


handle_irq_bcm2711:
  stp     x29, x30, [sp, #-16]!                   // Push frame pointer, procedure link register on stack.
  mov     x29, sp                                 // Update frame pointer to new stack location.
  adrp    x1, 0xfe00b000 + _start
  ldr     w0, [x1, 0x200]                         // w0 = [0xfe00b200]
  cmp     w0, #2
  b.ne    1f
  bl      handle_timer_irq
.if UART_DEBUG
  b       2f
.endif
1:
.if UART_DEBUG
  bl      log_unknown_interrupt_value
2:
.endif
  ldp     x29, x30, [sp], #16                     // Pop frame pointer, procedure link register off stack.
  ret


.if UART_DEBUG
log_unknown_interrupt_value:
  stp     x29, x30, [sp, #-16]!                   // Push frame pointer, procedure link register on stack.
  mov     x29, sp                                 // Update frame pointer to new stack location.
  adr     x1, msg_unknown_interrupt_value
  mov     x2, #32
  bl      hex_x0
  adr     x0, msg_unknown_interrupt
  bl      uart_puts
  ldp     x29, x30, [sp], #16                     // Pop frame pointer, procedure link register off stack.
  ret


msg_unknown_interrupt:
  .ascii "Unknown interrupt: 0x"                  // concatenates with the string below
msg_unknown_interrupt_value:
  .asciz "........\r\n"                           // stops (.) are replaced with value in hex_x0 routine
.endif