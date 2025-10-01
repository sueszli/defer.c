#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    printf("Hello, World!\n");

    // undefined behavior: integer overflow
    int x = __INT_MAX__;
    int y = x + 1;
    printf("overflow result: %d\n", y);

    // memory leak
    volatile char *leak1 = malloc(100000);
    volatile char *leak2 = malloc(100000);
    volatile char *leak3 = malloc(100000);
    volatile char *leak4 = malloc(100000);

    if (leak1) {
        leak1[0] = 'a';
        char val = leak1[0];
        (void)val;
    }
    if (leak2) {
        leak2[0] = 'b';
        char val = leak2[0];
        (void)val;
    }
    if (leak3) {
        leak3[0] = 'c';
        char val = leak3[0];
        (void)val;
    }
    if (leak4) {
        leak4[0] = 'd';
        char val = leak4[0];
        (void)val;
    }

    return EXIT_SUCCESS;
}
