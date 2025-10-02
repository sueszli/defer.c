#include "defer.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    char *mem = malloc(100);
    defer({
        free(mem);
        printf("mem freed\n");
    });

    printf("mem allocated\n");
    return EXIT_SUCCESS;
}
