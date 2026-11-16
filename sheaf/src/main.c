#include "async.h"
#include "benchmark.h"
#include "go.h"
#include "tqdm.h"
#include "types.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static u32 compute_task_count = 12;
static u64 prime_limit = 10000000;

static volatile u32 compute_progress_go = 0;
static volatile u32 compute_progress_async = 0;

static bool is_prime(u64 n) {
    if (n < 2)
        return false;
    if (n == 2)
        return true;
    if (n % 2 == 0)
        return false;

    for (u64 i = 3; i * i <= n; i += 2) {
        if (n % i == 0)
            return false;
    }
    return true;
}

static void count_primes_go(void) {
    static u32 worker_id = 1;
    u32 my_id = __atomic_fetch_add(&worker_id, 1, __ATOMIC_SEQ_CST);

    u64 start = (my_id - 1) * (prime_limit / compute_task_count);
    u64 end = my_id * (prime_limit / compute_task_count);
    u32 count = 0;

    for (u64 n = start; n < end; n++) {
        if (is_prime(n))
            count++;
    }

    __atomic_fetch_add(&compute_progress_go, 1, __ATOMIC_SEQ_CST);
}

static void count_primes_async(void) {
    static u32 worker_id = 1;
    u32 my_id = __atomic_fetch_add(&worker_id, 1, __ATOMIC_SEQ_CST);

    u64 start = (my_id - 1) * (prime_limit / compute_task_count);
    u64 end = my_id * (prime_limit / compute_task_count);
    u32 count = 0;

    for (u64 n = start; n < end; n++) {
        if (is_prime(n))
            count++;

        if (n % 50000 == 0) {
            async_yield();
        }
    }

    __atomic_fetch_add(&compute_progress_async, 1, __ATOMIC_SEQ_CST);
}

static void test_compute_heavy_go(void) {
    compute_progress_go = 0;
    for (u32 i = 0; i < compute_task_count; i++) {
        spawn(count_primes_go);
    }

    while (compute_progress_go < compute_task_count) {
        tqdm(compute_progress_go, compute_task_count, "go   ", "workers");
        usleep(10000);
    }
    wait();
    tqdm(compute_task_count, compute_task_count, "go   ", "workers");
}

static void test_compute_heavy_async(void) {
    compute_progress_async = 0;
    for (u32 i = 0; i < compute_task_count; i++) {
        async_spawn(count_primes_async);
    }

    async_run_all();
    tqdm(compute_task_count, compute_task_count, "async", "workers");
}

int main(void) {
    f64 compute_go_time = benchmark_silent({ test_compute_heavy_go(); });
    f64 compute_async_time = benchmark_silent({ test_compute_heavy_async(); });

    printf("results: go in %.3fs vs async in %.3fs (%.1fx %s)\n", compute_go_time, compute_async_time, compute_go_time < compute_async_time ? compute_async_time / compute_go_time : compute_go_time / compute_async_time, compute_go_time < compute_async_time ? "faster go" : "faster async");

    return EXIT_SUCCESS;
}
