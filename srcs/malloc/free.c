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
/*      Filename: free.c                                                      */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/11/22 12:08:27 by espadara                              */
/*      Updated: 2025/11/23 17:56:48 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_malloc.h"

static void free_slab_block(t_slab *slab, void *ptr, int type, int class_idx)
{
  size_t      offset;
  int         block_idx;
  int         bitmap_idx;
  int         bit_pos;

  offset = (char *)ptr - ((char *)slab + sizeof(t_slab));
  block_idx = offset / slab->block_size;

  // which int and which bit in that int?
  bitmap_idx = block_idx / 64;
  bit_pos = block_idx % 64;

  slab->bitmap[bitmap_idx] &= ~(1ULL << bit_pos);
  slab->free_count++;

  if (slab->free_count == slab->total_blocks)
    {
      if (slab->prev)
        slab->prev->next = slab->next;
      else
        {
          if (type == 0) g_heap.tiny[class_idx] = slab->next;
          else           g_heap.small[class_idx] = slab->next;
        }
      if (slab->next)
        slab->next->prev = slab->prev;

      size_t zone_size = (type == 0) ? TINY_ZONE_SIZE : SMALL_ZONE_SIZE;
      munmap(slab, zone_size);
    }
}

static bool free_large(void *ptr)
{
  t_slab *slab = g_heap.large;
  void *data_start;

  while (slab)
    {
      data_start = (void *)(slab + 1);
      if (ptr == data_start)
        {
          // Unlink from active list
          if (slab->prev)
            slab->prev->next = slab->next;
          else
            g_heap.large = slab->next;
          if (slab->next)
            slab->next->prev = slab->prev;

          // CACHING LOGIC
          if (g_heap.cache_count < CACHE_SIZE)
          {
            slab->prev = NULL;
            // Add to cache list head
            slab->next = g_heap.cache_large;
            if (g_heap.cache_large)
              g_heap.cache_large->prev = slab;
            g_heap.cache_large = slab;
            g_heap.cache_count++;
          }
          else
            {
              // Cache full, really unmap
              size_t total_size = slab->block_size + sizeof(t_slab);
              // Re-align to page size just to be safe for munmap length
              total_size = (total_size + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1);
              munmap(slab, total_size);
          }
          return (true);
        }
      slab = slab->next;
    }
  return (false);
}

__attribute__((visibility("default")))
void sea_free(void *ptr)
{
  t_slab  *slab;
  int     type = 0;
  int     class_idx = 0;

  if (!ptr)
    return;

  pthread_mutex_lock(&g_malloc_mutex);

  slab = find_slab_by_ptr(ptr, &type);
  if (slab)
    {
      // Reverse engineer the class index from block_size
      // Size 32 -> (32 / 16) - 1 = Index 1
      class_idx = (slab->block_size / MIN_ALIGNMENT) - 1;
      free_slab_block(slab, ptr, type, class_idx);
      pthread_mutex_unlock(&g_malloc_mutex);
      return;
    }
  if (free_large(ptr))
    {
      pthread_mutex_unlock(&g_malloc_mutex);
      return;
    }
  pthread_mutex_unlock(&g_malloc_mutex);
}
