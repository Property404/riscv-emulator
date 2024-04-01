#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

[[noreturn]] extern void _exit(int);
extern long write(int, const char*, int);

void exit(int code) {
    _exit(code);
}


void __assert(bool condition, const char* message) {
    if (!condition) {
        puts(message);
        exit(1);
    }
}

size_t strlen(const char *s) {
    assert(s != NULL);

    unsigned count = 0;
    for (count = 0; s[count] != '\0'; count++) { }
    return count;
}

int puts(const char* buf) {
    assert(buf != NULL);

    write(1, buf, strlen(buf));
    putchar('\n');
    return 0;
}

int putchar(int c) {
    char ch = c;
    write(1, &ch, 1);
    return 0;
}

__asm__(".globl _exit\n\t"
        ".type _exit, @function\n\t"
        "_exit:\n\t"
        ".cfi_startproc\n\t"
        "addi a7, x0, 93\n\t"
        "ecall\n\t"
        "ret\n\t"
        ".cfi_endproc");

__asm__(".globl _write\n\t"
        ".type _write, @function\n\t"
        "write:\n\t"
        ".cfi_startproc\n\t"
        "addi a7, x0, 64\n\t"
        "ecall\n\t"
        "ret\n\t"
        ".cfi_endproc");
