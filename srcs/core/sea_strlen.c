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
/*      Filename: sea_strlen.c                                                */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/08/26 13:41:00 by espadara                              */
/*      Updated: 2025/11/23 17:25:13 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_core.h"

size_t	sea_strlen(const char *s)
{
  const char      *char_ptr;
  const __m128i   *longword_ptr;
  __m128i         longword;
  __m128i         zero;
  unsigned int    mask;

    // Align to 16-byte boundary
    char_ptr = s;
    while (((uintptr_t)char_ptr & 15) != 0)
    {
        if (*char_ptr == '\0')
            return (char_ptr - s);
        char_ptr++;
    }

    // 2. SIMD Loop (16 bytes at a time)
    longword_ptr = (const __m128i *)char_ptr;
    zero = _mm_setzero_si128();

    while (1)
    {
        // Load 16 bytes from memory2
        longword = _mm_load_si128(longword_ptr);
        // Compare bytes against zero. Result is 0xFF where match, 0x00 otherwise.
        longword = _mm_cmpeq_epi8(longword, zero);
        mask = _mm_movemask_epi8(longword);

        if (mask != 0)
            return ((char *)longword_ptr - s + __builtin_ctz(mask));

        longword_ptr++;
    }
}
