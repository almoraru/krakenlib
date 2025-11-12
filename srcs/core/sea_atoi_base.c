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
/*      Filename: sea_atoi_base.c                                             */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/08/28 23:36:41 by espadara                              */
/*      Updated: 2025/08/28 23:46:52 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_core.h"


int	sea_atoi_base(const char *str, int base)
{
  int result = 0;
  int sign = 1;
  int digit_value;

    if (!str || base < 2 || base > 16)
      return (result);
    while (sea_iswhitespace((unsigned char)*str))
      str++;
    if (*str == '-' || *str == '+')
      {
        if (*str == '-')
          sign = -1;
        str++;
      }
    while (*str)
      {
        if (*str >= '0' && *str <= '9')
          digit_value = *str - '0';
        else if (*str >= 'a' && *str <= 'f')
          digit_value = *str - 'a' + 10;
        else if (*str >= 'A' && *str <= 'F')
          digit_value = *str - 'A' + 10;
        else
          break;
        if (digit_value >= base)
          break;
        result = (result * base) + digit_value;
        str++;
      }
    return (result * sign);
}
