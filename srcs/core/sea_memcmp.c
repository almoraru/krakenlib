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
/*      Filename: sea_memcmp.c                                                */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/08/28 16:53:50 by espadara                              */
/*      Updated: 2025/11/23 18:33:09 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_core.h"

int	sea_memcmp(const void *s1, const void *s2, size_t n)
{
    const unsigned char *p1 = (const unsigned char *)s1;
    const unsigned char *p2 = (const unsigned char *)s2;

    if (n < 16)
        goto tail;

    const __m128i *v1 = (const __m128i *)p1;
    const __m128i *v2 = (const __m128i *)p2;
    __m128i val1, val2, cmp;
    unsigned int mask;

    while (n >= 16)
    {
        // Load 16 bytes (unaligned load is safe on modern CPUs)
        val1 = _mm_loadu_si128(v1);
        val2 = _mm_loadu_si128(v2);

        // Compare bytes for equality (0xFF if equal, 0x00 if not)
        cmp = _mm_cmpeq_epi8(val1, val2);

        // Create bitmask from comparison result
        // 0xFFFF means all 16 bytes were equal
        mask = _mm_movemask_epi8(cmp);

        if (mask != 0xFFFF)
        {
            unsigned int diff_mask = (~mask) & 0xFFFF;
            int index = __builtin_ctz(diff_mask); // Index of first diff

            const unsigned char *c1 = (const unsigned char *)v1;
            const unsigned char *c2 = (const unsigned char *)v2;
            return (c1[index] - c2[index]);
        }

        v1++;
        v2++;
        n -= 16;
    }
    p1 = (const unsigned char *)v1;
    p2 = (const unsigned char *)v2;

tail:
    while (n--)
    {
        if (*p1 != *p2)
            return (*p1 - *p2);
        p1++;
        p2++;
    }
    return (0);
}
