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
/*      Updated: 2025/11/24 16:04:02 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_core.h"

int	sea_strncmp(const char *s1, const char *s2, size_t n)
{
    if (n == 0)
        return (0);

    const unsigned char *p1 = (const unsigned char *)s1;
    const unsigned char *p2 = (const unsigned char *)s2;
    __m128i v1, v2, zero;
    unsigned int mask_zero, mask_eq;

    // Align to 16-byte
    while (n > 0 && ((uintptr_t)p1 & 15) != 0)
    {
        if (*p1 != *p2 || *p1 == '\0')
            return (*p1 - *p2);
        p1++;
        p2++;
        n--;
    }

    if (n >= 16)
    {
        zero = _mm_setzero_si128();
        while (n >= 16)
        {
            // Load 16 bytes
            v1 = _mm_loadu_si128((const __m128i *)p1);
            v2 = _mm_loadu_si128((const __m128i *)p2);

            // Check for Null Terminator
            __m128i cmp_zero = _mm_cmpeq_epi8(v1, zero);
            mask_zero = _mm_movemask_epi8(cmp_zero);

            // Check for Equality
            __m128i cmp_eq = _mm_cmpeq_epi8(v1, v2);
            mask_eq = _mm_movemask_epi8(cmp_eq);

            // If difference found OR null terminator found
            if (mask_eq != 0xFFFF || mask_zero != 0)
            {
                // Find exact position
                for (int i = 0; i < 16; i++)
                {
                    if (p1[i] != p2[i] || p1[i] == '\0')
                        return (p1[i] - p2[i]);
                }
            }

            p1 += 16;
            p2 += 16;
            n -= 16;
        }
    }

    while (n-- > 0)
    {
        if (*p1 != *p2 || *p1 == '\0')
            return (*p1 - *p2);
        p1++;
        p2++;
    }

    return (0);
}
