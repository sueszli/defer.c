#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    printf("Hello, World!\n");

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

    char *overflow = malloc(10);
    overflow[10] = 'x';  // one byte past the allocation
    
    char *underflow = malloc(10);
    underflow[-1] = 'y';  // one byte before the allocation
    
    char *dangling = malloc(50);
    free(dangling);
    dangling[0] = 'z';  // accessing freed memory
    
    char *doublefree = malloc(50);
    free(doublefree);
    free(doublefree);  // freeing the same pointer twice

    return EXIT_SUCCESS;
}
