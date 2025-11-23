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
/*      Updated: 2025/11/23 16:52:28 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_malloc.h"

static inline bool is_in_slab(t_slab *slab, void *ptr)
{
    void *start;
    void *end;

    start = (void *)((char *)slab + sizeof(t_slab));
    end = (void *)((char *)start + (slab->total_blocks * slab->block_size));

    return (ptr >= start && ptr < end);
}

t_slab *find_slab_by_ptr(void *ptr, int *type_out)
{
  int i;
  t_slab *slab;

  for (i = 0; i < MAX_TINY_CLASSES; i++)
    {
      slab = g_heap.tiny[i];
      while (slab)
        {
          if (is_in_slab(slab, ptr))
            {
              if (type_out) *type_out = 0;
              return (slab);
            }
          slab = slab->next;
        }
    }
  for (i = 0; i < MAX_SMALL_CLASSES; i++)
    {
      slab = g_heap.small[i];
      while (slab)
        {
          if (is_in_slab(slab, ptr))
            {
              if (type_out) *type_out = 1;
              return (slab);
            }
          slab = slab->next;
        }
    }
  return (NULL);
}
