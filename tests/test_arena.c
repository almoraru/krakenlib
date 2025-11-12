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
/*      Filename: arena_test.c                                                */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/11/08 17:01:49 by espadara                              */
/*      Updated: 2025/11/13 00:20:40 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>    // For printf, sprintf
#include <stdlib.h>   // For NULL, size_t (though sea_lib.h has it)
#include <string.h>   // For sprintf (test harness only)
#include <assert.h>   // For testing
#include <stdint.h>   // For uintptr_t
#include <stdalign.h> // For alignof

// --- CHANGE THIS ---
// Include your main library header file here
#include "krakenlib.h"
// ---------------

// --- Test 1: Basic Allocation & Data Integrity ---
void test_basic_allocation() {
    printf("--- Test 1: Basic Allocation ---\n");
    t_mem *arena = sea_arena_init(ARENA_DEFAULT);
    assert(arena != NULL);

    const char *test_str = "Hello, Arena!";
    // Use the library's strlen
    size_t len = sea_strlen(test_str) + 1; // +1 for null terminator

    char *s = sea_arena_alloc(arena, len);
    assert(s != NULL); // Allocation should not fail

    // Use the library's memcpy
    sea_memcpy(s, test_str, len);

    // Use the library's strcmp
    assert(sea_strcmp(s, "Hello, Arena!") == 0);

    printf("  Basic allocation OK.\n");
    printf("  String written: \"%s\"\n", s);

    sea_arena_free(arena);
    printf("  Arena freed.\n\n");
}

// --- Test 2: Multiple Small Allocations & Data Integrity ---
void test_multiple_allocations() {
    printf("--- Test 2: Multiple Small Allocations ---\n");
    t_mem *arena = sea_arena_init(ARENA_DEFAULT);
    assert(arena != NULL);

    #define NUM_ALLOCS 100
    char *ptrs[NUM_ALLOCS];

    // 1. Allocate
    for (int i = 0; i < NUM_ALLOCS; i++) {
        ptrs[i] = sea_arena_alloc(arena, 32); // Allocate 32 bytes for each
        assert(ptrs[i] != NULL);
        // Write a unique string to each (using stdio sprintf)
        sprintf(ptrs[i], "Test string %d", i);
    }

    // 2. Verify
    // Check that no allocation overwrote a previous one
    for (int i = 0; i < NUM_ALLOCS; i++) {
        char expected_str[32];
        sprintf(expected_str, "Test string %d", i);
        // Use the library's strcmp
        assert(sea_strcmp(ptrs[i], expected_str) == 0);
    }

    printf("  %d allocations OK.\n", NUM_ALLOCS);
    printf("  Data integrity verified.\n");

    sea_arena_free(arena);
    printf("  Arena freed.\n\n");
}

// --- Test 3: Large Allocation ---
void test_large_allocation() {
    printf("--- Test 3: Large Allocation ---\n");
    t_mem *arena = sea_arena_init(ARENA_DEFAULT);
    assert(arena != NULL);

    // 10 MB allocation
    size_t large_size = 10 * 1024 * 1024;
    unsigned char *large_block = sea_arena_alloc(arena, large_size);
    assert(large_block != NULL);

    // Write to the beginning and end to check bounds
    large_block[0] = 0xDE;
    large_block[large_size - 1] = 0xAD;

    // Verify
    assert(large_block[0] == 0xDE);
    assert(large_block[large_size - 1] == 0xAD);

    printf("  10MB allocation OK.\n");
    printf("  Wrote to and verified start and end of block.\n");

    sea_arena_free(arena);
    printf("  Arena freed.\n\n");
}

// --- Test 4: Block-Crossing Allocation ---
void test_block_crossing_allocation() {
    printf("--- Test 4: Block-Crossing Allocation ---\n");

    // Use the define from your header
    const size_t ARENA_BLOCK_SIZE = ARENA_DEFAULT;

    t_mem *arena = sea_arena_init(ARENA_DEFAULT);
    assert(arena != NULL);

    // Allocate slightly less than one block
    size_t size1 = ARENA_BLOCK_SIZE - 100;
    char *p1 = sea_arena_alloc(arena, size1);
    assert(p1 != NULL);
    sea_strcpy(p1, "Block 1");

    // Allocate more. This should "cross" the boundary and
    // force a new block to be created (assuming your logic is right).
    size_t size2 = 200;
    char *p2 = sea_arena_alloc(arena, size2);
    assert(p2 != NULL);
    sea_strcpy(p2, "Block 2");

    // The most important check: is the *first* block still valid?
    assert(sea_strcmp(p1, "Block 1") == 0);
    assert(sea_strcmp(p2, "Block 2") == 0);

    printf("  Allocated %zu bytes, then %zu bytes.\n", size1, size2);
    printf("  Verified data in both blocks is intact.\n");

    sea_arena_free(arena);
    printf("  Arena freed.\n\n");
}

// --- Test 5: Alignment Test ---
void test_alignment() {
    printf("--- Test 5: Alignment Test ---\n");
    t_mem *arena = sea_arena_init(ARENA_DEFAULT);
    assert(arena != NULL);

    // Your header guarantees ARENA_ALIGN (16).
    // These tests check if the pointer is valid for basic types.
    // A 16-byte alignment will pass all of these.

    // Allocate 1 byte to misalign the "next" pointer (potentially)
    sea_arena_alloc(arena, 1);

    // Test alignment for various types
    int *p_int = sea_arena_alloc(arena, sizeof(int));
    assert((uintptr_t)p_int % alignof(int) == 0);
    *p_int = 12345;

    sea_arena_alloc(arena, 3); // More misalignment

    double *p_double = sea_arena_alloc(arena, sizeof(double));
    assert((uintptr_t)p_double % alignof(double) == 0);
    *p_double = 6.789;

    sea_arena_alloc(arena, 5); // More misalignment

    void **p_ptr = sea_arena_alloc(arena, sizeof(void*));
    assert((uintptr_t)p_ptr % alignof(void*) == 0);
    *p_ptr = (void*)0xDEADBEEF;

    // Verify all data
    assert(*p_int == 12345);
    assert(*p_double == 6.789);
    assert(*p_ptr == (void*)0xDEADBEEF);

    printf("  Verified alignment for int, double, and void*.\n");
    printf("  Data integrity after aligned allocs OK.\n");

    sea_arena_free(arena);
    printf("  Arena freed.\n\n");
}

// --- Test 6: Zero-Byte Allocation ---
void test_zero_byte_allocation() {
    printf("--- Test 6: Zero-Byte Allocation ---\n");
    t_mem *arena = sea_arena_init(ARENA_DEFAULT);
    assert(arena != NULL);

    // We just want to ensure it doesn't crash.
    void *p = sea_arena_alloc(arena, 0);

    // A subsequent allocation should still work.
    char *s = sea_arena_alloc(arena, 10);
    assert(s != NULL);
    sea_strcpy(s, "After zero");

    assert(sea_strcmp(s, "After zero") == 0);

    printf("  sea_arena_alloc(0) did not crash.\n");
    printf("  Subsequent allocation OK.\n");

    sea_arena_free(arena);
    (void)p;
    printf("  Arena freed.\n\n");
}

// --- Test 7: Stress Test ---
void test_stress() {
    printf("--- Test 7: Stress Test ---\n");
    t_mem *arena = sea_arena_init(ARENA_DEFAULT);
    assert(arena != NULL);

    #define STRESS_ALLOCS 100000
    for (int i = 0; i < STRESS_ALLOCS; i++) {
        size_t size_to_alloc = (i % 128) + 1;
        void *p = sea_arena_alloc(arena, size_to_alloc);
        assert(p != NULL);
    }

    printf("  Completed %d mixed-size allocations.\n", STRESS_ALLOCS);

    sea_arena_free(arena);
    printf("  Arena freed.\n\n");
}

// --- Test 8: Arena-Specific String Functions ---
void test_arena_specific_functions() {
    printf("--- Test 8: Arena-Specific String Functions ---\n");
    t_mem *arena = sea_arena_init(ARENA_DEFAULT);
    assert(arena != NULL);

    // 1. Test sea_arena_strdup
    const char *s1 = "Hello";
    char *dup_s1 = sea_arena_strdup(arena, s1);
    assert(dup_s1 != s1); // Must be a new pointer
    assert(sea_strcmp(dup_s1, "Hello") == 0);
    printf("  sea_arena_strdup OK.\n");

    // 2. Test sea_arena_strjoin
    const char *s2 = ", World!";
    char *joined = sea_arena_strjoin(arena, dup_s1, s2);
    assert(sea_strcmp(joined, "Hello, World!") == 0);
    printf("  sea_arena_strjoin OK.\n");

    // 3. Test sea_arena_split
    char **split_res = sea_arena_split(arena, "one two three", ' ');
    assert(split_res != NULL);
    assert(sea_strcmp(split_res[0], "one") == 0);
    assert(sea_strcmp(split_res[1], "two") == 0);
    assert(sea_strcmp(split_res[2], "three") == 0);
    assert(split_res[3] == NULL);
    printf("  sea_arena_split OK.\n");

    // 4. Test arena_strsub
    char *sub = sea_arena_strsub(arena, "0123456789", 2, 5);
    assert(sea_strcmp(sub, "23456") == 0);
    printf("  sea_arena_strsub OK.\n");

    // All this memory was allocated on the arena.
    // Let's do one more small allocation to ensure the
    // arena state (used_mem, etc.) is still valid.
    char *final_check = sea_arena_alloc(arena, 10);
    sea_strcpy(final_check, "all good");
    assert(sea_strcmp(final_check, "all good") == 0);
    printf("  Final allocation check OK.\n");

    sea_arena_free(arena);
    printf("  Arena freed.\n\n");
}


// --- Main Test Runner ---
int main() {
    printf("=== Starting 'sea' Arena Allocator Test Suite ===\n\n");

    test_basic_allocation();
    test_multiple_allocations();
    test_large_allocation();
    test_block_crossing_allocation();
    test_alignment();
    test_zero_byte_allocation();
    test_stress();
    test_arena_specific_functions();

    printf("=== All Tests Passed ===\n");
    printf("Remember to run this with Valgrind!\n");
    return 0;
}
