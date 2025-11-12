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
/*      Filename: sea_strmapi.c                                               */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/09/01 22:38:42 by espadara                              */
/*      Updated: 2025/09/01 22:39:54 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_core.h"

char	*sea_strmapi(char const *s, char (*f)(unsigned int, char))
{
  char *new_str;
  size_t len;
  unsigned int i;

  if (!s || !f)
    return (NULL);
  len = sea_strlen(s);
  new_str = malloc(sizeof(char) * (len + 1));
  if (!new_str)
    return (NULL);
  i = 0;
  while (i < len)
    {
      new_str[i] = f(i, s[i]);
      i++;
    }
  new_str[i] = '\0';
  return (new_str);
}
