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
/*      Updated: 2025/11/22 11:17:17 by espadara                              */
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

    void *ptr1 = sea_malloc(10);
    void *ptr2 = sea_malloc(100);
    void *ptr3 = sea_malloc(1000);
    void *ptr4 = sea_malloc(10000);

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

    sea_free(ptr1);
    sea_free(ptr2);
    sea_free(ptr3);
    sea_free(ptr4);

    printf("  ✅ Basic allocations work!\n");
}

void test_zone_boundaries(void)
{
    printf("\n🔹 TEST 2: Zone Boundaries\n");

    // Test TINY zone boundary (64 bytes)
    void *tiny_max = sea_malloc(64);
    void *tiny_over = sea_malloc(65);

    // Test SMALL zone boundary (1024 bytes)
    void *small_max = sea_malloc(1024);
    void *small_over = sea_malloc(1025);

    assert(tiny_max != NULL);
    assert(tiny_over != NULL);
    assert(small_max != NULL);
    assert(small_over != NULL);

    printf("  TINY max (64):    %p\n", tiny_max);
    printf("  TINY over (65):   %p\n", tiny_over);
    printf("  SMALL max (1024): %p\n", small_max);
    printf("  SMALL over (1025): %p\n", small_over);

    sea_free(tiny_max);
    sea_free(tiny_over);
    sea_free(small_max);
    sea_free(small_over);

    printf("  ✅ Zone boundaries work!\n");
}

void test_zero_and_null(void)
{
    printf("\n🔹 TEST 3: Zero Size and NULL\n");

    void *zero = sea_malloc(0);
    printf("  sea_malloc(0) returned: %p\n", zero);

    sea_free(NULL);  // Should not crash
    printf("  sea_free(NULL) didn't crash\n");

    sea_free(zero);

    printf("  ✅ Edge cases handled!\n");
}

void test_realloc(void)
{
    printf("\n🔹 TEST 4: Realloc Tests\n");

    char *ptr = sea_malloc(50);
    printf("  sea_malloc(50) = %p\n", ptr);
    assert(ptr != NULL);

    strcpy(ptr, "Hello, Kraken!");
    printf("  Original (50 bytes): '%s' at %p\n", ptr, ptr);

    // Grow
    printf("  Calling sea_realloc(%p, 100)...\n", ptr);
    ptr = sea_realloc(ptr, 100);
    printf("  sea_realloc(100) returned: %p\n", ptr);
    assert(ptr != NULL);
    assert(strcmp(ptr, "Hello, Kraken!") == 0);
    printf("  After sea_realloc(100): '%s'\n", ptr);

    // Shrink
    printf("  Calling sea_realloc(%p, 20)...\n", ptr);
    ptr = sea_realloc(ptr, 20);
    printf("  sea_realloc(20) returned: %p\n", ptr);
    assert(ptr != NULL);
    printf("  After sea_realloc(20): '%s'\n", ptr);

    // NULL realloc (should act like malloc)
    printf("  Calling sea_realloc(NULL, 50)...\n");
    void *new_ptr = sea_realloc(NULL, 50);
    printf("  sea_realloc(NULL, 50) returned: %p\n", new_ptr);
    assert(new_ptr != NULL);
    sea_free(new_ptr);

    sea_free(ptr);

    printf("  ✅ Realloc works!\n");
}

void test_calloc(void)
{
    printf("\n🔹 TEST 5: Calloc (zero-initialized)\n");

    int *arr = sea_calloc(10, sizeof(int));
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

    sea_free(arr);
}

void test_fragmentation(void)
{
    printf("\n🔹 TEST 6: Fragmentation Test\n");

    void *ptrs[100];

    // Allocate 100 small blocks
    for (int i = 0; i < 100; i++) {
        ptrs[i] = sea_malloc(32);
        assert(ptrs[i] != NULL);
    }

    // Free every other block (create holes)
    for (int i = 0; i < 100; i += 2) {
        sea_free(ptrs[i]);
    }

    // Allocate more (should reuse freed blocks)
    for (int i = 0; i < 100; i += 2) {
        ptrs[i] = sea_malloc(32);
        assert(ptrs[i] != NULL);
    }

    // Free all
    for (int i = 0; i < 100; i++) {
        sea_free(ptrs[i]);
    }

    printf("  ✅ Fragmentation handled!\n");
}

void test_large_allocations(void)
{
    printf("\n🔹 TEST 7: Large Allocations\n");

    void *huge1 = sea_malloc(1024 * 1024);      // 1 MB
    void *huge2 = sea_malloc(1024 * 1024 * 5);  // 5 MB

    assert(huge1 != NULL);
    assert(huge2 != NULL);

    printf("  1 MB allocation:  %p\n", huge1);
    printf("  5 MB allocation:  %p\n", huge2);

    memset(huge1, 0xFF, 1024 * 1024);
    memset(huge2, 0xAA, 1024 * 1024 * 5);

    assert(((unsigned char*)huge1)[0] == 0xFF);
    assert(((unsigned char*)huge2)[0] == 0xAA);

    sea_free(huge1);
    sea_free(huge2);

    printf("  ✅ Large allocations work!\n");
}

void test_stress(void)
{
    printf("\n🔹 TEST 8: Stress Test (%d allocations)\n", STRESS_TEST_COUNT);

    void *ptrs[STRESS_TEST_COUNT];

    for (int i = 0; i < STRESS_TEST_COUNT; i++) {
        // Random sizes
        size_t size = (i % 10) * 100 + 10;
        ptrs[i] = sea_malloc(size);
        assert(ptrs[i] != NULL);
        memset(ptrs[i], i % 256, size);
    }

    // Free in reverse order
    for (int i = STRESS_TEST_COUNT - 1; i >= 0; i--) {
        sea_free(ptrs[i]);
    }

    printf("  ✅ Stress test passed!\n");
}

void test_original_demo(void)
{
    printf("\n🔹 TEST 10: Original Demo\n");

    void *ptr[1024];

    for (int i = 0; i < 20; i++)
    {
        if ((i % 2) == 0)
            ptr[i] = sea_malloc(600);
        else if ((i % 5) == 0)
            ptr[i] = sea_malloc(5000);
        else if ((i % 3) == 0)
            ptr[i] = sea_malloc(200);
        else if ((i % 8) == 0)
            ptr[i] = sea_malloc(1000);
        else if ((i % 7) == 0)
            ptr[i] = sea_malloc(10000000);
        else
            ptr[i] = sea_malloc(50);
    }

    show_alloc_mem();

    sea_printf("\n------------BONUS------------\n");
    char *poof = "cocococococcococo1231232132132131ZAAAAAAAAAWAAAAAAAAAAAAAAAAAAAAAARUUUUUUUUUUUUUUUDOOOOOOOOOOOOOOO";
    int len = sea_strlen(poof);
    sea_memcpy(ptr[3], (void *)poof, len);
    show_alloc_mem_ex(ptr[3]);

    for (int i = 0; i < 20; i++)
        sea_free(ptr[i]);

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
    test_original_demo();

    printf("\n");
    printf("🐙 ============================================== 🐙\n");
    printf("       🎉 ALL MALLOC TESTS PASSED! 🎉\n");
    printf("       The Kraken's memory is UNLEASHED!\n");
    printf("🐙 ============================================== 🐙\n");
    printf("\n");

    return 0;
}
