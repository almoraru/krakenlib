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
/*      Filename: sea_strlcat.c                                               */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/08/27 20:01:39 by espadara                              */
/*      Updated: 2025/08/27 20:11:12 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_core.h"

size_t	sea_strlcat(char *dst, const char *src, size_t size)
{
  size_t dst_len = 0;
  size_t src_len = sea_strlen(src);
  size_t i = 0;

  while (dst[dst_len] && dst_len < size)
    dst_len++;
  if (dst_len >= size)
    return(size + src_len);
  while (src[i] && (dst_len + i) < (size - 1))
    {
      dst[dst_len + i] = src[i];
      i++;
    }
  dst[dst_len + i] = 0;
  return (dst_len + src_len);
}
