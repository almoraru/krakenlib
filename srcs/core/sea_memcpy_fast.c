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
/*      Updated: 2025/11/23 18:19:07 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_core.h"

void	*sea_memcpy_fast(void *dest, const void *src, size_t n)
{
    unsigned char       *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;

    // Handle small sizes (< 16 bytes) immediately
    if (n < 16)
        {
            while (n--)
                *d++ = *s++;
            return (dest);
        }

    // SIMD Loop (Unrolled 4x = 64 bytes per iteration)
    __m128i *d_vec = (__m128i *)d;
    const __m128i *s_vec = (const __m128i *)s;

    while (n >= 64)
    {
        // Load 64 bytes
        __m128i r0 = _mm_loadu_si128(s_vec + 0);
        __m128i r1 = _mm_loadu_si128(s_vec + 1);
        __m128i r2 = _mm_loadu_si128(s_vec + 2);
        __m128i r3 = _mm_loadu_si128(s_vec + 3);

        // Store 64 bytes
        _mm_storeu_si128(d_vec + 0, r0);
        _mm_storeu_si128(d_vec + 1, r1);
        _mm_storeu_si128(d_vec + 2, r2);
        _mm_storeu_si128(d_vec + 3, r3);

        s_vec += 4;
        d_vec += 4;
        n -= 64;
    }

    //Handle remaining 16-byte chunks
    while (n >= 16)
    {
        _mm_storeu_si128(d_vec, _mm_loadu_si128(s_vec));
        s_vec++;
        d_vec++;
        n -= 16;
    }
// TAIL
    d = (unsigned char *)d_vec;
    s = (const unsigned char *)s_vec;
    while (n--)
        *d++ = *s++;

    return (dest);
}
