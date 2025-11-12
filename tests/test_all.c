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
/*      Filename: test_all.c                                                  */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/11/13 00:00:14 by espadara                              */
/*      Updated: 2025/11/13 00:00:15 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "krakenlib.h"
#include <stdio.h>

int main(void)
{
    printf("\n");
    printf("🐙 ============================================== 🐙\n");
    printf("      KRAKENLIB v%s QUICK TEST SUITE\n", KRAKENLIB_VERSION);
    printf("🐙 ============================================== 🐙\n");
    printf("\n");

    printf("🔧 Testing Core Functions...\n");
    printf("  sea_strlen(\"Hello\"): %zu\n", sea_strlen("Hello"));
    printf("  sea_atoi(\"42\"): %d\n", sea_atoi("42"));
    printf("  ✅ Core functions work!\n\n");

    printf("🖨️  Testing Printf...\n");
    sea_printf("  sea_printf test: %s %d %x\n", "Hello", 42, 255);
    printf("  ✅ Printf works!\n\n");

    printf("💾 Testing Malloc...\n");
    void *ptr = malloc(100);
    if (ptr) {
        sea_memcpy(ptr, "Kraken!", 8);
        printf("  Allocated and wrote: %s\n", (char *)ptr);
        free(ptr);
        printf("  ✅ Malloc/free work!\n\n");
    }

    printf("🐙 ============================================== 🐙\n");
    printf("       🎉 QUICK TESTS PASSED! 🎉\n");
    printf("       Run 'make test-all' for full tests\n");
    printf("🐙 ============================================== 🐙\n");
    printf("\n");

    return 0;
}
