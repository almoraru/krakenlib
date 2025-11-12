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
/*      Updated: 2025/11/13 00:16:42 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_core.h"

t_mem	*sea_arena_init(size_t size)
{
t_mem *arena;

    if (size == 0)
        size = ARENA_DEFAULT;
    arena = mmap(NULL, sizeof(t_mem) + size, PROT_READ | PROT_WRITE,
                 MAP_ANON | MAP_PRIVATE, -1, 0);
    if (arena == MAP_FAILED)
        return NULL;

    arena->next = NULL;
    arena->total = size;
    arena->used = 0;
    arena->mem = (unsigned char *)(arena + 1);

    return arena;
}
