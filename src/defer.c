#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    printf("Hello, World!\n");
    char *leak = malloc(100); // this should leak
    return EXIT_SUCCESS;
}
