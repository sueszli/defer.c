#include "defer.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {

    char *mem = malloc(100000);
    defer({
        free(mem);
        printf("freed mem\n");
    });

    printf("allocated mem\n");

    return EXIT_SUCCESS;
}
