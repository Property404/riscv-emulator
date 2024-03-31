.option norvc
.global _start
.section .rodata
hello: .ascii "Hello World!\n"
.section .text

_start:
# write
li s1, 0x4
1:
addi a0, x0, 1 # stdout
la a1, hello
addi a2, x0, 13
addi a7, x0, 64
ecall
addi s1, s1, -1
bne x0, s1, 1b

# Exit
addi a0, x0, 0 
addi a7, x0, 93
ecall
