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
/*      Filename: sea_strcmp.c                                                */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/08/27 23:06:03 by espadara                              */
/*      Updated: 2025/11/23 17:25:26 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_core.h"

int	sea_strcmp(const char *s1, const char *s2)
{
    // Optimization Strategy:
    // Standard libc often loads chunks and checks for \0 and diff simultaneously.
    // To reuse our fast memcmp, we can cheat slightly for aligned comparisons,
    // but the safest portable way using our existing tools is:
    // 1. For very short strings, overhead of strlen calls might not be worth it.
    // But for benchmarks with long strings, this is a win.
    // Option A: Just use sea_memcmp safely?
    // We don't know the safe length. We must loop.
    // BUT we can write a dedicated SIMD loop that checks for \0 AND diff.

    const unsigned char *p1 = (const unsigned char *)s1;
    const unsigned char *p2 = (const unsigned char *)s2;
    __m128i v1, v2, zero;
    unsigned int mask_zero, mask_eq;

    // Check alignment (optional but good) or handle small strings
    // Loop until aligned or difference found or end
    while (((uintptr_t)p1 & 15) != 0)
    {
        if (*p1 != *p2 || *p1 == '\0')
            return (*p1 - *p2);
        p1++;
        p2++;
    }
    zero = _mm_setzero_si128();
    while (1)
    {
        // Load 16 bytes
        v1 = _mm_loadu_si128((const __m128i *)p1);
        v2 = _mm_loadu_si128((const __m128i *)p2);

        // Check for Null Terminator in v1
        // 0xFF where bytes are zero
        __m128i cmp_zero = _mm_cmpeq_epi8(v1, zero);
        mask_zero = _mm_movemask_epi8(cmp_zero);

        // Check for Equality
        // 0xFF where bytes are equal
        __m128i cmp_eq = _mm_cmpeq_epi8(v1, v2);
        mask_eq = _mm_movemask_epi8(cmp_eq);

        if (mask_eq != 0xFFFF || mask_zero != 0)
        {
            // We found a difference OR a null terminator.
            // We must find which one happened *first*.
            // Iterate the 16 bytes to find the exact spot
            // (We could use bit twiddling, but a loop of 16 is fast enough here
            // since this branch is only taken once at the end).
            const unsigned char *c1 = (const unsigned char *)&v1;
            const unsigned char *c2 = (const unsigned char *)&v2;

            for (int i = 0; i < 16; i++)
            {
                if (c1[i] != c2[i] || c1[i] == '\0')
                    return (c1[i] - c2[i]);
            }
        }

        p1 += 16;
        p2 += 16;
    }
}
