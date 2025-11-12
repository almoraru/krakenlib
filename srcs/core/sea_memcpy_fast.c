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
/*      Filename: sea_memcpy_fast.c                                           */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/08/29 23:47:00 by espadara                              */
/*      Updated: 2025/08/29 23:50:36 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_core.h"

void	*sea_memcpy_fast(void *dest, const void *src, size_t n)
{
    if (n < 16)
    {
        unsigned char *d = dest;
        const unsigned char *s = src;
        while (n--)
            *d++ = *s++;
        return (dest);
    }
    const __m128i *a = (const __m128i *)src;
    __m128i *b = (__m128i *)dest;
    while (n >= 16)
    {
        _mm_storeu_si128(b++, _mm_loadu_si128(a++));
        n -= 16;
    }
    const unsigned char *a2 = (const unsigned char *)a;
    unsigned char *b2 = (unsigned char *)b;
    while (n--)
        *b2++ = *a2++;

    return (dest)
;
}
