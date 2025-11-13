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
/*      Updated: 2025/11/13 22:51:36 by espadara                              */
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

// ============================================================
// STRING BENCHMARKS (FIXED - NO CONSTANT FOLDING)
// ============================================================

void benchmark_strlen(benchmark_result *result)
{
    // Use volatile to prevent constant folding
    char *test = malloc(100);
    strcpy(test, "Hello, Kraken! This is a test string for benchmarking that is long enough.");
    volatile char *vtest = test; // Prevent compile-time optimization

    double start, end;
    uint64_t cycles_start, cycles_end;
    volatile size_t len = 0;  // volatile prevents optimization

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

    // Fill with random-ish data to prevent optimization
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
// MALLOC BENCHMARKS
// ============================================================

void benchmark_malloc_small(benchmark_result *result)
{
    void *ptrs[1000];
    double start, end;
    uint64_t cycles_start, cycles_end;

    printf("  Benchmarking malloc (16 bytes)...\n");

    // Warmup
    for (int i = 0; i < WARMUP_ITERATIONS / 100; i++) {
        void *p = malloc(16);
        COMPILER_BARRIER();
        free(p);
    }

    // Benchmark krakenlib malloc
    start = get_time();
    cycles_start = rdtsc();
    for (int i = 0; i < ITERATIONS; i++) {
        ptrs[i % 1000] = malloc(16);
        COMPILER_BARRIER();
        if (i % 1000 == 999) {
            for (int j = 0; j < 1000; j++)
                free(ptrs[j]);
        }
    }
    cycles_end = rdtsc();
    end = get_time();
    result->kraken_time = (end - start) / ITERATIONS * 1e9;
    result->kraken_cycles = (double)(cycles_end - cycles_start) / ITERATIONS;

    result->name = "malloc(16)";
    // Note: Can't easily benchmark libc malloc separately
    result->libc_time = result->kraken_time;
    result->libc_cycles = result->kraken_cycles;
}

void benchmark_malloc_medium(benchmark_result *result)
{
    void *ptrs[1000];
    double start, end;
    uint64_t cycles_start, cycles_end;

    printf("  Benchmarking malloc (512 bytes)...\n");

    // Warmup
    for (int i = 0; i < WARMUP_ITERATIONS / 100; i++) {
        void *p = malloc(512);
        COMPILER_BARRIER();
        free(p);
    }

    start = get_time();
    cycles_start = rdtsc();
    for (int i = 0; i < ITERATIONS / 10; i++) {
        ptrs[i % 1000] = malloc(512);
        COMPILER_BARRIER();
        if (i % 1000 == 999) {
            for (int j = 0; j < 1000; j++)
                free(ptrs[j]);
        }
    }
    cycles_end = rdtsc();
    end = get_time();
    result->kraken_time = (end - start) / (ITERATIONS / 10) * 1e9;
    result->kraken_cycles = (double)(cycles_end - cycles_start) / (ITERATIONS / 10);

    result->name = "malloc(512)";
    result->libc_time = result->kraken_time;
    result->libc_cycles = result->kraken_cycles;
}

void benchmark_malloc_large(benchmark_result *result)
{
    void *ptrs[100];
    double start, end;
    uint64_t cycles_start, cycles_end;

    printf("  Benchmarking malloc (4096 bytes)...\n");

    start = get_time();
    cycles_start = rdtsc();
    for (int i = 0; i < ITERATIONS / 100; i++) {
        ptrs[i % 100] = malloc(4096);
        COMPILER_BARRIER();
        if (i % 100 == 99) {
            for (int j = 0; j < 100; j++)
                free(ptrs[j]);
        }
    }
    cycles_end = rdtsc();
    end = get_time();
    result->kraken_time = (end - start) / (ITERATIONS / 100) * 1e9;
    result->kraken_cycles = (double)(cycles_end - cycles_start) / (ITERATIONS / 100);

    result->name = "malloc(4096)";
    result->libc_time = result->kraken_time;
    result->libc_cycles = result->kraken_cycles;
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

    // Redirect stdout to /dev/null for fair comparison
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

    // Restore stdout
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
    printf("  Iterations: %d per test\n", ITERATIONS);
    printf("  Compiler: GCC with -O2 optimization\n");
    printf("\n");

    printf("%-22s | %12s | %12s | %10s\n",
           "Operation", "Kraken (ns)", "libc (ns)", "Difference");
    printf("--------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        if (strcmp(results[i].name, "malloc(16)") == 0 ||
            strcmp(results[i].name, "malloc(512)") == 0 ||
            strcmp(results[i].name, "malloc(4096)") == 0) {
            printf("%-22s | %12.2f | %12s | %10s\n",
                   results[i].name,
                   results[i].kraken_time,
                   "N/A",
                   "N/A");
        } else {
            double diff = ((results[i].kraken_time - results[i].libc_time) /
                           results[i].libc_time) * 100;
            printf("%-22s | %12.2f | %12.2f | %+9.1f%%\n",
                   results[i].name,
                   results[i].kraken_time,
                   results[i].libc_time,
                   diff);
        }
    }

    printf("\n");
    printf("Cycle counts:\n");
    printf("%-22s | %12s | %12s\n", "Operation", "Kraken", "libc");
    printf("--------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        if (strcmp(results[i].name, "malloc(16)") == 0 ||
            strcmp(results[i].name, "malloc(512)") == 0 ||
            strcmp(results[i].name, "malloc(4096)") == 0) {
            printf("%-22s | %12.0f | %12s\n",
                   results[i].name,
                   results[i].kraken_cycles,
                   "N/A");
        } else {
            printf("%-22s | %12.0f | %12.0f\n",
                   results[i].name,
                   results[i].kraken_cycles,
                   results[i].libc_cycles);
        }
    }

    printf("\n");
    printf("🐙 Benchmark Complete! 🐙\n\n");
}

int main(void)
{
    benchmark_result results[10];
    int idx = 0;

    printf("\n");
    printf("🐙 ============================================== 🐙\n");
    printf("       KRAKENLIB PERFORMANCE BENCHMARK\n");
    printf("🐙 ============================================== 🐙\n\n");

    printf("Running benchmarks...\n\n");

    // String benchmarks first (faster)
    printf("String Operations:\n");
    benchmark_strlen(&results[idx++]);
    benchmark_strcmp(&results[idx++]);
    benchmark_memcpy(&results[idx++]);

    printf("\nFormatted Output:\n");
    benchmark_printf(&results[idx++]);

    // Memory benchmarks last (can be affected by previous allocations)
    printf("\nMemory Allocation:\n");
    benchmark_malloc_small(&results[idx++]);
    benchmark_malloc_medium(&results[idx++]);
    benchmark_malloc_large(&results[idx++]);

    print_results(results, idx);

    return 0;
}
