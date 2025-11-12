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
/*      Filename: utils.c                                                     */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/11/11 23:00:59 by espadara                              */
/*      Updated: 2025/11/11 23:00:59 by espadara                              */
/*                                                                            */
/* ************************************************************************** */


#include "sea_malloc.h"

static void prefix_nbr(char *buff, int size)
{
  if (size == 2)
  {
    buff[0] = '0';
    buff[1] = 'b';
  }
  else if (size == 16)
  {
    buff[0] = '0';
    buff[1] = 'x';
  }
}

void ft_putsizebase(size_t nb, int size)
{
  static const char base[17] = {"0123456789abcdef"};
  char buff[128];
  long power;
  size_t tmp;
  int i;

  if (size < 2 || size > 16)
    return;
  power = 1;
  tmp = nb;
  while (tmp /= size)
    power *= size;
  if (size == 2 || size == 16)
    i = 2;
  else
    i = 0;
  prefix_nbr(buff, size);
  while (power)
  {
    buff[i++] = base[nb / power];
    nb -= (nb / power) * power;
    power /= size;
  }
  write(1, buff, i);
}

void putaddr(void *addr)
{
  ft_putsizebase((size_t)addr, 16);
}

/*
** ---------- Align mem on mask+1. Mask+1 is a power of 2 ----------
*/
size_t align_mem(size_t size, size_t mask)
{
  return ((size + mask) & ~mask);
}

/*
** ---------- TINY = 0, SMALL = 1, LARGE = 2 ----------
*/
size_t page_size(size_t size)
{
  static const size_t msize[3] = {MALLOC_TINY, MALLOC_SMALL,
                                  MALLOC_LARGE};

  return (msize[(size > ZONE_TINY) + (size > ZONE_SMALL)]);
}
