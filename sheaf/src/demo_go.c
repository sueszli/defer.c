#include "defer.h"
#include "go.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

static void task(u32 id) {
    usleep((u32)((rand() % 1000) * 1000));
    printf("finished task %u\n", id);
}

i32 main(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    srand((u32)tv.tv_sec + (u32)tv.tv_usec);

    u32 *results = malloc(4 * sizeof(u32));
    defer({
        free(results);
        printf("cleaned up\n");
    });

    go({ task(1); });
    go({ task(2); });
    go({ task(3); });

    wait();

    return EXIT_SUCCESS;
}
