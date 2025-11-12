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
/*      Filename: sea_strnstr.c                                               */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/08/27 23:03:05 by espadara                              */
/*      Updated: 2025/08/27 23:17:14 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_core.h"

char	*sea_strnstr(const char *haystack, const char *needle, size_t len)
{
  size_t needle_len = sea_strlen(needle);

  if (*needle == 0)
    return ((char*)haystack);

  while (*haystack && len-- >= needle_len)
    {
      if (*haystack == *needle)
        {
          if (sea_strncmp(haystack, needle, needle_len) == 0)
            return ((char*)haystack);
        }
      haystack++;
    }

  return (NULL);
}
