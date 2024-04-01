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

int main() {
    puts("Hello, world!\n");
}
