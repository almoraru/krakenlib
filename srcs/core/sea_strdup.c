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
/*      Filename: sea_strdup.c                                                */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/08/29 23:59:53 by espadara                              */
/*      Updated: 2025/08/30 00:07:48 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_core.h"


char *sea_arena_strdup(t_mem *arena, const char *src)
{
  if (!arena || !src)
    return (NULL);

  size_t len = sea_strlen(src) + 1;

  char *dest = sea_arena_alloc(arena, len);
  if (!dest)
    return (NULL);
  return (sea_memcpy_fast(dest, src, len));
}

char *sea_strdup(const char *s)
{
  if (s == NULL)
    return (NULL);
  size_t len = sea_strlen(s) + 1;
  char *dest = malloc(len);

  if (!dest)
    return (NULL);
  return (sea_memcpy_fast(dest, s, len));
}
