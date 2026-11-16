#include "async.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>

static void ping(void) {
    for (u8 i = 0; i < 2; i++) {
        printf("ping\n");
        async_yield();
    }
}

static void pong(void) {
    for (u8 i = 0; i < 2; i++) {
        printf("pong\n");
        async_yield();
    }
}

i32 main(void) {
    async_spawn(ping);
    async_spawn(pong);
    async_run_all();
    return EXIT_SUCCESS;
}
