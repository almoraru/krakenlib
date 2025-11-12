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
/*      Filename: sea_strjoin.c                                               */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/08/30 17:20:59 by espadara                              */
/*      Updated: 2025/08/30 17:35:10 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_core.h"

char	*sea_strjoin(char const *s1, char const *s2)
{
  char *str = NULL;
  size_t s1_len;
  size_t s2_len;
  if (!s1 || !s2)
    return (NULL);
  s1_len = sea_strlen(s1);
  s2_len = sea_strlen(s2);
  str = malloc(s1_len + s2_len + 1);
  if (!str)
    return (NULL);
  sea_memcpy_fast(str, s1, s1_len);
  sea_memcpy_fast(str + s1_len, s2, s2_len);
  str[s1_len + s2_len] = 0;

  return (str);
}

char	*sea_arena_strjoin(t_mem *arena, char const *s1, char const *s2)
{
  char *str = NULL;
  size_t s1_len;
  size_t s2_len;
  if (!s1 || !s2)
    return (NULL);
  s1_len = sea_strlen(s1);
  s2_len = sea_strlen(s2);
  str = sea_arena_alloc(arena, s1_len + s2_len + 1);
  if (!str)
    return (NULL);
  sea_memcpy_fast(str, s1, s1_len);
  sea_memcpy_fast(str + s1_len, s2, s2_len);
  str[s1_len + s2_len] = 0;

  return (str);
}
