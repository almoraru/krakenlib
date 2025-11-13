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
/*      Filename: test.c                                                      */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/11/11 22:41:16 by espadara                              */
/*      Updated: 2025/11/13 08:09:26 by espadara                              */
/*                                                                            */
/* ************************************************************************** */
#include "krakenlib.h"
#include <stdio.h>

#include "krakenlib.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define TEST_COUNT 50
#define STRESS_TEST_COUNT 1000

void test_basic_allocations(void)
{
    printf("\n🔹 TEST 1: Basic Allocations\n");

    void *ptr1 = malloc(10);
    void *ptr2 = malloc(100);
    void *ptr3 = malloc(1000);
    void *ptr4 = malloc(10000);

    assert(ptr1 != NULL);
    assert(ptr2 != NULL);
    assert(ptr3 != NULL);
    assert(ptr4 != NULL);

    // Write to them
    memset(ptr1, 'A', 10);
    memset(ptr2, 'B', 100);
    memset(ptr3, 'C', 1000);
    memset(ptr4, 'D', 10000);

    // Verify
    assert(((char*)ptr1)[0] == 'A');
    assert(((char*)ptr2)[0] == 'B');
    assert(((char*)ptr3)[0] == 'C');
    assert(((char*)ptr4)[0] == 'D');

    free(ptr1);
    free(ptr2);
    free(ptr3);
    free(ptr4);

    printf("  ✅ Basic allocations work!\n");
}

void test_zone_boundaries(void)
{
    printf("\n🔹 TEST 2: Zone Boundaries\n");

    // Test TINY zone boundary (64 bytes)
    void *tiny_max = malloc(64);
    void *tiny_over = malloc(65);

    // Test SMALL zone boundary (1024 bytes)
    void *small_max = malloc(1024);
    void *small_over = malloc(1025);

    assert(tiny_max != NULL);
    assert(tiny_over != NULL);
    assert(small_max != NULL);
    assert(small_over != NULL);

    printf("  TINY max (64):    %p\n", tiny_max);
    printf("  TINY over (65):   %p\n", tiny_over);
    printf("  SMALL max (1024): %p\n", small_max);
    printf("  SMALL over (1025): %p\n", small_over);

    free(tiny_max);
    free(tiny_over);
    free(small_max);
    free(small_over);

    printf("  ✅ Zone boundaries work!\n");
}

void test_zero_and_null(void)
{
    printf("\n🔹 TEST 3: Zero Size and NULL\n");

    void *zero = malloc(0);
    printf("  malloc(0) returned: %p\n", zero);

    free(NULL);  // Should not crash
    printf("  free(NULL) didn't crash\n");

    free(zero);

    printf("  ✅ Edge cases handled!\n");
}

void test_realloc(void)
{
    printf("\n🔹 TEST 4: Realloc Tests\n");

    char *ptr = malloc(50);
    strcpy(ptr, "Hello, Kraken!");

    printf("  Original (50 bytes): '%s'\n", ptr);

    // Grow
    ptr = realloc(ptr, 100);
    assert(ptr != NULL);
    assert(strcmp(ptr, "Hello, Kraken!") == 0);
    printf("  After realloc(100): '%s'\n", ptr);

    // Shrink
    ptr = realloc(ptr, 20);
    assert(ptr != NULL);
    printf("  After realloc(20): '%s'\n", ptr);

    // NULL realloc (should act like malloc)
    void *new_ptr = realloc(NULL, 50);
    assert(new_ptr != NULL);
    free(new_ptr);

    free(ptr);

    printf("  ✅ Realloc works!\n");
}

void test_calloc(void)
{
    printf("\n🔹 TEST 5: Calloc (zero-initialized)\n");

    int *arr = calloc(10, sizeof(int));
    assert(arr != NULL);

    int all_zero = 1;
    for (int i = 0; i < 10; i++) {
        if (arr[i] != 0) {
            all_zero = 0;
            break;
        }
    }

    assert(all_zero);
    printf("  ✅ Calloc zeros memory!\n");

    free(arr);
}

void test_fragmentation(void)
{
    printf("\n🔹 TEST 6: Fragmentation Test\n");

    void *ptrs[100];

    // Allocate 100 small blocks
    for (int i = 0; i < 100; i++) {
        ptrs[i] = malloc(32);
        assert(ptrs[i] != NULL);
    }

    // Free every other block (create holes)
    for (int i = 0; i < 100; i += 2) {
        free(ptrs[i]);
    }

    // Allocate more (should reuse freed blocks)
    for (int i = 0; i < 100; i += 2) {
        ptrs[i] = malloc(32);
        assert(ptrs[i] != NULL);
    }

    // Free all
    for (int i = 0; i < 100; i++) {
        free(ptrs[i]);
    }

    printf("  ✅ Fragmentation handled!\n");
}

void test_large_allocations(void)
{
    printf("\n🔹 TEST 7: Large Allocations\n");

    void *huge1 = malloc(1024 * 1024);      // 1 MB
    void *huge2 = malloc(1024 * 1024 * 5);  // 5 MB

    assert(huge1 != NULL);
    assert(huge2 != NULL);

    printf("  1 MB allocation:  %p\n", huge1);
    printf("  5 MB allocation:  %p\n", huge2);

    memset(huge1, 0xFF, 1024 * 1024);
    memset(huge2, 0xAA, 1024 * 1024 * 5);

    assert(((unsigned char*)huge1)[0] == 0xFF);
    assert(((unsigned char*)huge2)[0] == 0xAA);

    free(huge1);
    free(huge2);

    printf("  ✅ Large allocations work!\n");
}

void test_stress(void)
{
    printf("\n🔹 TEST 8: Stress Test (%d allocations)\n", STRESS_TEST_COUNT);

    void *ptrs[STRESS_TEST_COUNT];

    for (int i = 0; i < STRESS_TEST_COUNT; i++) {
        // Random sizes
        size_t size = (i % 10) * 100 + 10;
        ptrs[i] = malloc(size);
        assert(ptrs[i] != NULL);
        memset(ptrs[i], i % 256, size);
    }

    // Free in reverse order
    for (int i = STRESS_TEST_COUNT - 1; i >= 0; i--) {
        free(ptrs[i]);
    }

    printf("  ✅ Stress test passed!\n");
}

void test_double_free_detection(void)
{
    printf("\n🔹 TEST 9: Double Free Detection\n");

    void *ptr = malloc(100);
    free(ptr);

    printf("  First free: OK\n");

    // This SHOULD be detected by your malloc (if implemented)
    // For now, just document it
    printf("  ⚠️  Double free detection not yet implemented\n");
    printf("  (Would crash on: free(ptr) again)\n");
}

void test_original_demo(void)
{
    printf("\n🔹 TEST 10: Original Demo\n");

    void *ptr[1024];

    for (int i = 0; i < 20; i++)
    {
        if ((i % 2) == 0)
            ptr[i] = malloc(600);
        else if ((i % 5) == 0)
            ptr[i] = malloc(5000);
        else if ((i % 3) == 0)
            ptr[i] = malloc(200);
        else if ((i % 8) == 0)
            ptr[i] = malloc(1000);
        else if ((i % 7) == 0)
            ptr[i] = malloc(10000000);
        else
            ptr[i] = malloc(50);
    }

    show_alloc_mem();

    sea_printf("\n------------BONUS------------\n");
    char *poof = "cocococococcococo1231232132132131ZAAAAAAAAAWAAAAAAAAAAAAAAAAAAAAAARUUUUUUUUUUUUUUUDOOOOOOOOOOOOOOO";
    int len = sea_strlen(poof);
    sea_memcpy(ptr[3], (void *)poof, len);
    show_alloc_mem_ex(ptr[3]);

    for (int i = 0; i < 20; i++)
        free(ptr[i]);

    printf("\n  ✅ Original demo passed!\n");
}

int main(void)
{
    printf("\n");
    printf("🐙 ============================================== 🐙\n");
    printf("       KRAKENLIB MALLOC COMPREHENSIVE TESTS\n");
    printf("🐙 ============================================== 🐙\n");

    test_basic_allocations();
    test_zone_boundaries();
    test_zero_and_null();
    test_realloc();
    test_calloc();
    test_fragmentation();
    test_large_allocations();
    test_stress();
    test_double_free_detection();
    test_original_demo();

    printf("\n");
    printf("🐙 ============================================== 🐙\n");
    printf("       🎉 ALL MALLOC TESTS PASSED! 🎉\n");
    printf("       The Kraken's memory is UNLEASHED!\n");
    printf("🐙 ============================================== 🐙\n");
    printf("\n");

    return 0;
}
