.option norvc
.global _start
.section .rodata
hello: .ascii "Hello World!\n"
.section .text

_start:
# write
addi a0, x0, 1 # stdout
la a1, hello
addi a2, x0, 13
addi a7, x0, 64
ecall

# Exit
addi a0, x0, 0 
addi a7, x0, 93
ecall
