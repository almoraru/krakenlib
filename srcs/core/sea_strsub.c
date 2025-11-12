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
/*      Filename: sea_strsub.c                                                */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/08/30 16:57:32 by espadara                              */
/*      Updated: 2025/08/30 17:09:17 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_core.h"

char	*sea_strsub(char const *s, unsigned int start, size_t len)
{
  char *sub;
  size_t s_len;

  if (!s)
    return (NULL);
  s_len = sea_strlen(s);
  if (start >= s_len)
    {
      sub = malloc(1);
      sub[0] = 0;
      return (sub);
    }
  if (s_len - start < len)
    len = s_len - start;
  sub = malloc(len + 1);
  if (!sub)
    return (NULL);
  sea_memcpy_fast(sub, s + start, len);
  sub[len] = 0;
  return (sub);
}

char	*sea_arena_strsub(t_mem *arena, char const *s, unsigned int start, size_t len)
{
    char    *sub;
    size_t  s_len;

    if (!arena || !s)
        return (NULL);

    s_len = strlen(s);
    if (start >= s_len)
        return (sea_arena_strdup(arena, ""));
    if (s_len - start < len)
        len = s_len - start;
    sub = sea_arena_alloc(arena, len + 1);
    if (!sub)
        return (NULL);
    sea_memcpy_fast(sub, s + start, len);
    sub[len] = '\0';
    return (sub);
}
