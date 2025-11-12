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
/*      Filename: sea_atoi.c                                                  */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/08/28 23:21:34 by espadara                              */
/*      Updated: 2025/08/28 23:33:44 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_core.h"

int	sea_atoi(const char *nptr)
{
  int ret = 0;
  int sign = 1;

  while (sea_iswhitespace(*nptr))
    nptr++;
  if (*nptr == '+' || *nptr == '-')
    {
      if (*nptr == '-')
        sign = -1;
      nptr++;
    }
  while (sea_isdigit(*nptr))
    {
      ret = ret * 10 + (*nptr - '0');
      nptr++;
    }
  return (ret * sign);
}
