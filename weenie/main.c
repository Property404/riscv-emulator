#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int panic(const char* msg) {
    puts(msg);
    exit(1);
    return 0;
}

void puthexdigit(uint8_t val) {
    if (val < 0xa) {
        putchar(val + '0');
    } else if(val < 0x10) {
        putchar(val + 'f');
    } else {
        panic("Not a hex digit");
    }
}

void puthex(uint32_t val) {
    puts("0x");
    for (int i=0;i<32;i+=4) {
        puthexdigit(((val >> (24 - i))&0xFF));
    }
}

int main() {
    for (int i=0; i<4; i++) {
        puthex(i);
        puts("Hello, world!\n");
        putchar('\n');
    }
}
