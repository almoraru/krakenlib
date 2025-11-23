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
/*      Filename: benchmark.c                                                 */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/11/13 22:35:31 by espadara                              */
/*      Updated: 2025/11/23 17:57:52 by espadara                              */
/*                                                                            */
/* ************************************************************************** */
#include "krakenlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>

#define ITERATIONS 1000000
#define WARMUP_ITERATIONS 100000

// High-precision timer
static inline double get_time(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

// Get CPU cycles (x86_64)
static inline uint64_t rdtsc(void)
{
    unsigned int lo, hi;
    __asm__ volatile ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

// Prevent compiler optimization
#define COMPILER_BARRIER() __asm__ volatile("" ::: "memory")

typedef struct {
    const char *name;
    double kraken_time;
    double libc_time;
    double kraken_cycles;
    double libc_cycles;
} benchmark_result;

// ============================================================
// STRING BENCHMARKS
// ============================================================

void benchmark_strlen(benchmark_result *result)
{
    char *test = malloc(100);
    strcpy(test, "Hello, Kraken! This is a test string for benchmarking that is long enough.");
    volatile char *vtest = test;

    double start, end;
    uint64_t cycles_start, cycles_end;
    volatile size_t len = 0;

    printf("  Benchmarking strlen...\n");

    // Warmup
    for (int i = 0; i < WARMUP_ITERATIONS; i++) {
        len = sea_strlen((char *)vtest);
    }

    // Kraken
    start = get_time();
    cycles_start = rdtsc();
    for (int i = 0; i < ITERATIONS; i++) {
        len = sea_strlen((char *)vtest);
    }
    cycles_end = rdtsc();
    end = get_time();
    result->kraken_time = (end - start) / ITERATIONS * 1e9;
    result->kraken_cycles = (double)(cycles_end - cycles_start) / ITERATIONS;

    // Libc
    start = get_time();
    cycles_start = rdtsc();
    for (int i = 0; i < ITERATIONS; i++) {
        len = strlen((char *)vtest);
    }
    cycles_end = rdtsc();
    end = get_time();
    result->libc_time = (end - start) / ITERATIONS * 1e9;
    result->libc_cycles = (double)(cycles_end - cycles_start) / ITERATIONS;

    result->name = "sea_strlen";
    free(test);
    (void)len;
}

void benchmark_strcmp(benchmark_result *result)
{
    char *s1 = malloc(100);
    char *s2 = malloc(100);
    strcpy(s1, "Hello, World! This is a longer test string for comparison.");
    strcpy(s2, "Hello, World! This is a longer test string for comparison.");

    volatile char *vs1 = s1;
    volatile char *vs2 = s2;

    double start, end;
    uint64_t cycles_start, cycles_end;
    volatile int cmp = 0;

    printf("  Benchmarking strcmp...\n");

    // Warmup
    for (int i = 0; i < WARMUP_ITERATIONS; i++) {
        cmp = sea_strcmp((char *)vs1, (char *)vs2);
    }

    // Kraken
    start = get_time();
    cycles_start = rdtsc();
    for (int i = 0; i < ITERATIONS; i++) {
        cmp = sea_strcmp((char *)vs1, (char *)vs2);
    }
    cycles_end = rdtsc();
    end = get_time();
    result->kraken_time = (end - start) / ITERATIONS * 1e9;
    result->kraken_cycles = (double)(cycles_end - cycles_start) / ITERATIONS;

    // Libc
    start = get_time();
    cycles_start = rdtsc();
    for (int i = 0; i < ITERATIONS; i++) {
        cmp = strcmp((char *)vs1, (char *)vs2);
    }
    cycles_end = rdtsc();
    end = get_time();
    result->libc_time = (end - start) / ITERATIONS * 1e9;
    result->libc_cycles = (double)(cycles_end - cycles_start) / ITERATIONS;

    result->name = "sea_strcmp";

    free(s1);
    free(s2);
    (void)cmp;
}

void benchmark_memcpy(benchmark_result *result)
{
    char *src = malloc(1024);
    char *dst = malloc(1024);

    for (int i = 0; i < 1024; i++) {
        src[i] = (char)(i * 37 + 17);
    }

    volatile char *vsrc = src;
    volatile char *vdst = dst;

    double start, end;
    uint64_t cycles_start, cycles_end;

    printf("  Benchmarking memcpy (1024 bytes)...\n");

    // Warmup
    for (int i = 0; i < WARMUP_ITERATIONS / 10; i++) {
        sea_memcpy_fast((char *)vdst, (char *)vsrc, 1024);
    }

    // Kraken
    start = get_time();
    cycles_start = rdtsc();
    for (int i = 0; i < ITERATIONS / 10; i++) {
        sea_memcpy_fast((char *)vdst, (char *)vsrc, 1024);
    }
    cycles_end = rdtsc();
    end = get_time();
    result->kraken_time = (end - start) / (ITERATIONS / 10) * 1e9;
    result->kraken_cycles = (double)(cycles_end - cycles_start) / (ITERATIONS / 10);

    // Libc
    start = get_time();
    cycles_start = rdtsc();
    for (int i = 0; i < ITERATIONS / 10; i++) {
        memcpy((char *)vdst, (char *)vsrc, 1024);
    }
    cycles_end = rdtsc();
    end = get_time();
    result->libc_time = (end - start) / (ITERATIONS / 10) * 1e9;
    result->libc_cycles = (double)(cycles_end - cycles_start) / (ITERATIONS / 10);

    result->name = "sea_memcpy_fast(1KB)";

    free(src);
    free(dst);
}

// ============================================================
// PRINTF BENCHMARK
// ============================================================

void benchmark_printf(benchmark_result *result)
{
    double start, end;
    uint64_t cycles_start, cycles_end;
    int fd_null;

    printf("  Benchmarking printf...\n");

    fd_null = open("/dev/null", O_WRONLY);

    // Warmup
    for (int i = 0; i < WARMUP_ITERATIONS / 1000; i++) {
        sea_printf("Test %d: %s = %x\n", i, "value", 0xDEADBEEF);
    }

    int stdout_backup = dup(STDOUT_FILENO);
    dup2(fd_null, STDOUT_FILENO);

    // Kraken
    start = get_time();
    cycles_start = rdtsc();
    for (int i = 0; i < ITERATIONS / 1000; i++) {
        sea_printf("Test %d: %s = %x\n", i, "value", 0xDEADBEEF);
        COMPILER_BARRIER();
    }
    cycles_end = rdtsc();
    end = get_time();
    result->kraken_time = (end - start) / (ITERATIONS / 1000) * 1e9;
    result->kraken_cycles = (double)(cycles_end - cycles_start) / (ITERATIONS / 1000);

    dup2(stdout_backup, STDOUT_FILENO);
    close(stdout_backup);

    // Libc
    start = get_time();
    cycles_start = rdtsc();
    for (int i = 0; i < ITERATIONS / 1000; i++) {
        dprintf(fd_null, "Test %d: %s = %x\n", i, "value", 0xDEADBEEF);
        COMPILER_BARRIER();
    }
    cycles_end = rdtsc();
    end = get_time();
    result->libc_time = (end - start) / (ITERATIONS / 1000) * 1e9;
    result->libc_cycles = (double)(cycles_end - cycles_start) / (ITERATIONS / 1000);

    close(fd_null);
    result->name = "sea_printf";
}

// ============================================================
// MALLOC BENCHMARK HELPER
// ============================================================

// Generic runner for malloc benchmarks to reduce code duplication
void run_malloc_bench(const char *name, size_t size, int divider, benchmark_result *result)
{
    void *ptrs[1000];
    double start, end;
    uint64_t cycles_start, cycles_end;
    int iters = ITERATIONS / divider;
    int batch_size = 100; // Reduced batch size for larger allocs

    if (size < 4096) batch_size = 1000;

    printf("  Benchmarking malloc (%zu bytes)...\n", size);

    // --- KRAKEN ---
    // Warmup
    for (int i = 0; i < WARMUP_ITERATIONS / 100; i++) {
        void *p = sea_malloc(size);
        COMPILER_BARRIER();
        sea_free(p);
    }

    start = get_time();
    cycles_start = rdtsc();
    for (int i = 0; i < iters; i++) {
        ptrs[i % batch_size] = sea_malloc(size);
        COMPILER_BARRIER();
        if (i % batch_size == (batch_size - 1)) {
            for (int j = 0; j < batch_size; j++)
                sea_free(ptrs[j]);
        }
    }
    cycles_end = rdtsc();
    end = get_time();
    result->kraken_time = (end - start) / iters * 1e9;
    result->kraken_cycles = (double)(cycles_end - cycles_start) / iters;

    // --- SYSTEM ---
    // Warmup
    for (int i = 0; i < WARMUP_ITERATIONS / 100; i++) {
        void *p = malloc(size);
        COMPILER_BARRIER();
        free(p);
    }

    start = get_time();
    cycles_start = rdtsc();
    for (int i = 0; i < iters; i++) {
        ptrs[i % batch_size] = malloc(size);
        COMPILER_BARRIER();
        if (i % batch_size == (batch_size - 1)) {
            for (int j = 0; j < batch_size; j++)
                free(ptrs[j]);
        }
    }
    cycles_end = rdtsc();
    end = get_time();
    result->libc_time = (end - start) / iters * 1e9;
    result->libc_cycles = (double)(cycles_end - cycles_start) / iters;

    result->name = name;
}

// ============================================================
// MAIN BENCHMARK RUNNER
// ============================================================

void print_results(benchmark_result *results, int count)
{
    printf("\n");
    printf("🐙 ============================================== 🐙\n");
    printf("              BENCHMARK RESULTS\n");
    printf("🐙 ============================================== 🐙\n\n");

    printf("System Info:\n");
    printf("  CPU: AMD Ryzen 9 7950X3D\n");
    printf("  Iterations: %d per test (scaled down for larger allocs)\n", ITERATIONS);
    printf("  Compiler: GCC with -O2 optimization\n");
    printf("\n");

    printf("%-22s | %12s | %12s | %10s\n",
           "Operation", "Kraken (ns)", "libc (ns)", "Difference");
    printf("--------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        double diff = ((results[i].kraken_time - results[i].libc_time) /
                       results[i].libc_time) * 100;
        printf("%-22s | %12.2f | %12.2f | %+9.1f%%\n",
               results[i].name,
               results[i].kraken_time,
               results[i].libc_time,
               diff);
    }

    printf("\n");
    printf("Cycle counts:\n");
    printf("%-22s | %12s | %12s\n", "Operation", "Kraken", "libc");
    printf("--------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        printf("%-22s | %12.0f | %12.0f\n",
               results[i].name,
               results[i].kraken_cycles,
               results[i].libc_cycles);
    }

    printf("\n");
    printf("🐙 Benchmark Complete! 🐙\n\n");
}

int main(void)
{
    benchmark_result results[20]; // Increased size for new tests
    int idx = 0;

    printf("\n");
    printf("🐙 ============================================== 🐙\n");
    printf("       KRAKENLIB PERFORMANCE BENCHMARK\n");
    printf("🐙 ============================================== 🐙\n\n");

    printf("Running benchmarks...\n\n");

    printf("String Operations:\n");
    benchmark_strlen(&results[idx++]);
    benchmark_strcmp(&results[idx++]);
    benchmark_memcpy(&results[idx++]);

    printf("\nFormatted Output:\n");
    benchmark_printf(&results[idx++]);

    printf("\nMemory Allocation (Small/Medium - Slab):\n");
    run_malloc_bench("malloc(16)", 16, 1, &results[idx++]);
    run_malloc_bench("malloc(512)", 512, 10, &results[idx++]);
    run_malloc_bench("malloc(4096)", 4096, 100, &results[idx++]);
    run_malloc_bench("malloc(8192)", 8192, 100, &results[idx++]); // Max Small

    printf("\nMemory Allocation (Large - Mmap/Cache):\n");
    run_malloc_bench("malloc(32KB)", 32 * 1024, 1000, &results[idx++]);
    run_malloc_bench("malloc(64KB)", 64 * 1024, 1000, &results[idx++]);
    run_malloc_bench("malloc(512KB)", 512 * 1024, 1000, &results[idx++]);
    run_malloc_bench("malloc(1MB)", 1024 * 1024, 2000, &results[idx++]);
    run_malloc_bench("malloc(16MB)", 16 * 1024 * 1024, 10000, &results[idx++]);

    print_results(results, idx);

    return 0;
}
