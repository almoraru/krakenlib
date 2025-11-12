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
/*      Filename: sea_strlcpy.c                                               */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/08/27 19:40:18 by espadara                              */
/*      Updated: 2025/08/27 19:52:19 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_core.h"

size_t	sea_strlcpy(char *dst, const char *src, size_t size)
{

  size_t source_len = sea_strlen(src);
  size_t len_to_copy = 0;

  if (size == 0)
    return (source_len);
  if (source_len < size)
    len_to_copy = source_len;
  else
    len_to_copy = size - 1;
  sea_memcpy(dst, src, len_to_copy);
  dst[len_to_copy] = 0;
  return (source_len);
}
