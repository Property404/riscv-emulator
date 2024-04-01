.option norvc
.global _start
.section .text

_start:
bne x0, sp, callmain
li sp, 0x91000
callmain:
call main

# Exit
addi a0, x0, 0 
addi a7, x0, 93
ecall
