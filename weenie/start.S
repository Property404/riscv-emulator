.option norvc
.global _start
.global sys_write
.section .rodata
hello: .ascii "Hello World!\n"
.section .text

_start:
li sp, 0x91000
call main

# Exit
addi a0, x0, 0 
addi a7, x0, 93
ecall

sys_write:
    addi a7, x0, 64
    ecall
    ret
