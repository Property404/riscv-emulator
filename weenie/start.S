.option norvc
.global _start
.global sys_write
.global sys_exit
.section .rodata
hello: .ascii "Hello World!\n"
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

sys_write:
    addi a7, x0, 64
    ecall
    ret
sys_exit:
    addi a7, x0, 93
    ecall
    ret
