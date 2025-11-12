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
/*      Filename: sea_memcpy.c                                                */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/08/26 22:43:15 by espadara                              */
/*      Updated: 2025/08/26 22:45:52 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_core.h"

void	*sea_memcpy(void *dest, const void *src, size_t n)
{
  unsigned char *d = dest;
  const unsigned char *s = src;
  const unsigned char *begin = s;

  while (s && (size_t)(s - begin) < n)
      *d++ = *s++;
    return (dest);
}
