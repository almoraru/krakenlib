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
/*      Filename: sea_strtok.c                                                */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/11/27 22:06:27 by espadara                              */
/*      Updated: 2025/11/27 22:07:23 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_core.h"

/* CAREFUL NOT THREAD SAFE */
char	*sea_strtok(char *str, const char *delim)
{
  static char	*next_token;
  char			*token_start;

  if (str)
    next_token = str;
  if (!next_token)
    return (NULL);

  while (*next_token && sea_strchr(delim, *next_token))
    next_token++;

  if (!*next_token)
	{
      next_token = NULL;
      return (NULL);
	}

  token_start = next_token;

  while (*next_token && !sea_strchr(delim, *next_token))
    next_token++;

  if (*next_token)
	{
      *next_token = '\0';
      next_token++;
	}
  else
    next_token = NULL;

  return (token_start);
}
