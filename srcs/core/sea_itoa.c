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
/*      Filename: sea_itoa.c                                                  */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/09/01 22:26:01 by espadara                              */
/*      Updated: 2025/09/01 22:35:26 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_core.h"

static int	count_digits(int n)
  {
    int count = 0;
    unsigned int temp_n = 0;
    if (n <= 0)
      count++;
    temp_n = (n < 0) ? -n : n;
    while (temp_n > 0)
      {
        temp_n /= 10;
        count++;
      }
    return (count);
  }
char	*sea_itoa(int n)
{
  int len;
  char *str = NULL;
  unsigned int num;

  if (n == INT_MIN)
    return(sea_strdup("-2147483648"));
  len = count_digits(n);
  str = malloc(sizeof(char) * (len + 1));
  if (!str)
    return (NULL);
  str[len] = 0;
  num = (n < 0) ? -n : n;
  if (num == 0)
    {
      str[0] = '0';
      return (str);
    }
  while (num > 0)
    {
      len--;
      str[len] = (num % 10) + '0';
      num /= 10;
    }
  if (n < 0)
    str[0] = '-';
  return (str);
}
