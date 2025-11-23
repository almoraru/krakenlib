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
/*      Filename: sea_memchr.c                                                */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/08/28 08:38:05 by espadara                              */
/*      Updated: 2025/11/23 17:35:10 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_core.h"

/*
** Optimized memchr using SSE2 and goto for clean tail handling.
*/
void	*sea_memchr(const void *s, int c, size_t n)
{
    const unsigned char *src = (const unsigned char *)s;
    unsigned char ch = (unsigned char)c;

    // Align to 16-byte boundary
    while (n > 0 && ((uintptr_t)src & 15))
    {
        if (*src == ch)
            return ((void *)src);
        src++;
        n--;
    }

    if (n < 16)
        goto tail;

    __m128i val, target, cmp;
    unsigned int mask;

    // Create a vector filled with the target char
    target = _mm_set1_epi8(ch);

    while (n >= 16)
      {
        val = _mm_load_si128((const __m128i *)src);

        // Compare: result bytes are 0xFF if match, 0x00 if not
        cmp = _mm_cmpeq_epi8(val, target);

        // Create bitmask
        mask = _mm_movemask_epi8(cmp);

        if (mask != 0)
        {
            // Found a match! ctz finds the first set bit index.
            return ((void *)(src + __builtin_ctz(mask)));
        }

        src += 16;
        n -= 16;
    }

tail:
    while (n-- > 0)
    {
        if (*src == ch)
            return ((void *)src);
        src++;
    }

    return (NULL);
}
