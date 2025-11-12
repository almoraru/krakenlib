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
/*      Filename: sea_atof.c                                                  */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/08/28 23:42:07 by espadara                              */
/*      Updated: 2025/08/28 23:46:26 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_core.h"

double	sea_atof(const char *str)
{
  double result = 0.0;
  double sign = 1.0;
  double power;


  while (sea_iswhitespace((unsigned char)*str))
    str++;
  if (*str == '-' || *str == '+')
    {
      if (*str == '-')
        sign = -1.0;
      str++;
    }
  while (sea_isdigit((unsigned char)*str))
    {
      result = result * 10.0 + (*str - '0');
      str++;
    }
  if (*str == '.')
    {
      str++;
      power = 1.0;
      while (sea_isdigit((unsigned char)*str))
        {
          power *= 10.0;
          result = result + (*str - '0') / power;
          str++;
        }
    }
  return (result * sign);
}
