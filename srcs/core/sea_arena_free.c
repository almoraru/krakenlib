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
/*      Filename: sea_arena_free.c                                            */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/08/29 23:21:46 by espadara                              */
/*      Updated: 2025/08/29 23:23:06 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_core.h"

void	sea_arena_free(t_mem *arena)
{
  t_mem *current = arena;
  while (current)
    {
      t_mem *next = current->next;
      free(current->mem);
      free(current);
      current =next;
    }
}
