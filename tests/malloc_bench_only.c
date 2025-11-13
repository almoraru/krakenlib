/* ************************************************************************** */
/*                                                                            */
/*                        ______                                              */
/*                     .-"      "-.                                           */
/*                    /            \                                          */
/*        _          |              |          _                              */
/*       ( \         |,  .-.  .-.  ,|         / )                             */
/*        > "=._     | )(__/  \__)( |     _.=" <                              */
/*       (_/"=._"=._ |/     /\     \| _.="_.="\_)                             */
/*              "=._ (_     ^^     _)"_.="                                    */
/*                  "=\__|IIIIII|__/="                                        */
/*                 _.="| \IIIIII/ |"=._                                       */
/*       _     _.="_.="\          /"=._"=._     _                             */
/*      ( \_.="_.="     `--------`     "=._"=._/ )                            */
/*       > _.="                            "=._ <                             */
/*      (_/                                    \_)                            */
/*                                                                            */
/*      Filename: malloc_bench_only.c                                         */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/11/13 22:53:06 by espadara                              */
/*      Updated: 2025/11/13 22:53:09 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

static inline double get_time(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

static inline uint64_t rdtsc(void)
{
    unsigned int lo, hi;
    __asm__ volatile ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

int main(void)
{
    void *ptrs[1000];
    double start, end;
    uint64_t cycles_start, cycles_end;
    const int ITERS = 1000000;

    printf("Malloc Benchmark:\n");

    // Small allocations
    start = get_time();
    cycles_start = rdtsc();
    for (int i = 0; i < ITERS; i++) {
        ptrs[i % 1000] = malloc(16);
        if (i % 1000 == 999) {
            for (int j = 0; j < 1000; j++)
                free(ptrs[j]);
        }
    }
    cycles_end = rdtsc();
    end = get_time();

    double time_ns = (end - start) / ITERS * 1e9;
    double cycles = (double)(cycles_end - cycles_start) / ITERS;

    printf("  malloc(16):   %.2f ns  (%.0f cycles)\n", time_ns, cycles);

    // Medium allocations
    start = get_time();
    cycles_start = rdtsc();
    for (int i = 0; i < ITERS / 10; i++) {
        ptrs[i % 1000] = malloc(512);
        if (i % 1000 == 999) {
            for (int j = 0; j < 1000; j++)
                free(ptrs[j]);
        }
    }
    cycles_end = rdtsc();
    end = get_time();

    time_ns = (end - start) / (ITERS / 10) * 1e9;
    cycles = (double)(cycles_end - cycles_start) / (ITERS / 10);

    printf("  malloc(512):  %.2f ns  (%.0f cycles)\n", time_ns, cycles);

    // Large allocations
    start = get_time();
    cycles_start = rdtsc();
    for (int i = 0; i < ITERS / 100; i++) {
        ptrs[i % 100] = malloc(4096);
        if (i % 100 == 99) {
            for (int j = 0; j < 100; j++)
                free(ptrs[j]);
        }
    }
    cycles_end = rdtsc();
    end = get_time();

    time_ns = (end - start) / (ITERS / 100) * 1e9;
    cycles = (double)(cycles_end - cycles_start) / (ITERS / 100);

    printf("  malloc(4096): %.2f ns  (%.0f cycles)\n", time_ns, cycles);

    return 0;
}
