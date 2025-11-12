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
/*      Filename: sea_arena_alloc.c                                           */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/08/29 23:15:38 by espadara                              */
/*      Updated: 2025/09/03 08:26:21 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_core.h"

void    *sea_arena_alloc(t_mem *arena, size_t size)
{
    if (!arena || size == 0)
        return (NULL);

    size_t aligned_size = (size + ARENA_ALIGN - 1) & ~(ARENA_ALIGN - 1);
    t_mem *current = arena;
    while (current)
    {
        if (current->total - current->used >= aligned_size)
        {
            void *ptr = current->mem + current->used;
            current->used += aligned_size;
            sea_bzero(ptr, aligned_size);
            return (ptr);
        }
        if (!current->next)
            break;
        current = current->next;
    }
    size_t new_block_size = (aligned_size > ARENA_DEFAULT) ? aligned_size : ARENA_DEFAULT;
    t_mem *new_block = sea_arena_init(new_block_size);
    if (!new_block)
        return (NULL);
    current->next = new_block;
    void *ptr = new_block->mem;
    new_block->used += aligned_size;
    sea_bzero(ptr, aligned_size);
    return (ptr);
}
