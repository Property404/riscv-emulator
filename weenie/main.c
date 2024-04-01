#include <stdint.h>

extern void sys_exit(int);
extern void sys_write(int fd, const char* buf, int count);

long unsigned strlen(const char* buf) {
    unsigned count = 0;
    for (count = 0;buf[count] != '\0'; count++) { }
    return count;
}

int puts(const char* buf) {
    sys_write(1, buf, strlen(buf));
    return 0;
}

int panic(const char* msg) {
    puts(msg);
    sys_exit(1);
    return 0;
}

int putchar(const char c) {
    sys_write(1, &c, 1);
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
