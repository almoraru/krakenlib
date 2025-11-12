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
/*      Filename: sea_strtrim.c                                               */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/09/01 00:10:18 by espadara                              */
/*      Updated: 2025/09/01 00:11:16 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_core.h"

char	*sea_strtrim(char const *s1, char const *set)
{
  char *trimmed_str;
  char *start;
  char *end;
  size_t len;

  if (!s1 || !set)
    return (NULL);
  start = (char *)s1;
  while (*start && sea_strchr(set, *start))
      start++;
  end = (char *)s1 + sea_strlen(s1) - 1;
  while (end > start && sea_strchr(set, *end))
    end--;
  len = (end - start) + 1;
  if (*start == '\0' || len <= 0)
    {
      trimmed_str = malloc(1);
      if (trimmed_str)
        trimmed_str[0] = '\0';
      return (trimmed_str);
    }

  trimmed_str = malloc(len + 1);
  if (!trimmed_str)
    return (NULL);

  sea_memcpy_fast(trimmed_str, start, len);
  trimmed_str[len] = '\0';
  return (trimmed_str);
}
