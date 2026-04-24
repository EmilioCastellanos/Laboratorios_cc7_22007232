.section .text
  .global _start

_start:
  b reset

reset:
  ldr sp, =0x82010000
  bl main
loop:
  b loop