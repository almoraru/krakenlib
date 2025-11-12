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
/*      Filename: sea_strstr.c                                                */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/08/27 22:54:25 by espadara                              */
/*      Updated: 2025/08/27 22:58:58 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_core.h"

char	*sea_strstr(const char *haystack, const char *needle)
{
  const char *h_ptr;
  const char *n_ptr;

  if (*needle == 0)
    return ((char *)haystack);
  while (*haystack)
    {
      if (*haystack == *needle)
        {
          h_ptr = haystack;
          n_ptr = needle;
          while (*n_ptr && *n_ptr == *h_ptr)
            {
              h_ptr++;
              n_ptr++;
            }
          if (*n_ptr == 0)
            return ((char *)haystack);
        }
      haystack++;
    }

  return (NULL);
}
