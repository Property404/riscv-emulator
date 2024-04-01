#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main() {
    for (int i=0; i<=15; i++) {
        if ( i % 3 == 0 && i % 5 != 0 ) {
            printf("Fizz\n");
        } else if ( i % 5 == 0 && i % 3 != 0 ) {
            printf("Buzz\n");
        } else if ( i % 3 == 0 && i % 5 == 0 ) {
            printf("FizzBuzz\n");
        } else {
            printf("%x\n", i);
        }
    }
}
