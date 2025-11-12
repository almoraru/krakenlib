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
/*      Filename: sea_arena_init.c                                            */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/08/29 23:14:34 by espadara                              */
/*      Updated: 2025/08/29 23:29:21 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_core.h"

t_mem	*sea_arena_init(size_t size)
{
  t_mem *arena = NULL;

  if (size == 0)
    size = ARENA_DEFAULT;
  arena = malloc(sizeof (t_mem));
  if (!arena)
    return (NULL);
  arena->mem = malloc(size);
  if (!arena->mem)
    {
      free(arena);
      return (NULL);
    }
  arena->next = NULL;
  arena->total = size;
  arena->used = 0;
  return (arena);
}
