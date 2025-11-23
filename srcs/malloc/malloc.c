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
/*      Filename: malloc.c                                                    */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/11/11 22:36:00 by espadara                              */
/*      Updated: 2025/11/23 16:52:06 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_malloc.h"

t_heap g_heap = {0};

pthread_mutex_t g_malloc_mutex = PTHREAD_MUTEX_INITIALIZER;

static inline int get_class_index(size_t size)
{
  if (size == 0)
    return (0);
  // Size 1-16 -> index 0 | size 17-32 -> index 1 etc.
  return ((size -1) / MIN_ALIGNMENT);
}

static t_slab *init_new_slab(int type, int class_index, size_t block_size)
{
  t_slab *slab;
  size_t zone_size;
  size_t available_bytes;
  t_slab **head;

  if (type == 0)// TINY
    {
      zone_size = TINY_ZONE_SIZE;
      head = &g_heap.tiny[class_index];
    }
 else // SMALL
   {
     zone_size = SMALL_ZONE_SIZE;
     head = &g_heap.small[class_index];
   }

  slab = mmap(NULL, zone_size, PROT_READ | PROT_WRITE,
              MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (slab == MAP_FAILED)
    return (NULL);

  slab->block_size = block_size;
  available_bytes = zone_size - sizeof(t_slab);
  slab->total_blocks = available_bytes / block_size;

  if (slab->total_blocks > 1024)
    slab->total_blocks = 1024;

  slab->free_count = slab->total_blocks;
  slab->prev = NULL;

  // Insert to the front of Global Heap
  slab->next = *head;
  if (*head)
    (*head)->prev = slab;
  *head = slab;

  return (slab);
}

static void *alloc_from_slab(t_slab *slab)
{
  int i;
  uint64_t inverted;
  int bit_pos;
  size_t global_pos;

  for (i = 0; i < 16; i++)
    {
      // UINT64_MAX -> ALL BITS ARE (1)
      if (slab->bitmap[i] != UINT64_MAX)
        {
          // Invert to find first (0)
          inverted = ~slab->bitmap[i];
          // __builtin_ffsll: Built-in CPU instruction to find index of first set bit
          bit_pos = __builtin_ffsll(inverted) - 1; // 0 - 63

          // Mark bit as used (1)
          slab->bitmap[i] |= (1ULL << bit_pos);
          slab->free_count--;

          //Adress = slab_start + header_size + (block_index * block_size)
          global_pos =  (i * 64) + bit_pos;

          /* [ SLAB HEADER ] [ BLOCK 0 ] [ BLOCK 1 ] [ BLOCK 2 ] [ BLOCK 3 ] ...
          ** ^               ^           ^           ^
          ** |               |           |           |
          ** Start (slab)    |           |           Target (Block 2)
          **                 |           |
          **                 End of Header
          */
          return ((void *) ((char *)slab +
                            sizeof(t_slab) + (global_pos * slab->block_size)));
        }
    }
  return (NULL);
}

static void *allocate_tiny_small(size_t size, int type)
  {
    int class_idx;
    size_t aligned_size;
    t_slab *slab;

    class_idx = get_class_index(size);

    // get true block size. Example: 17 -> aligned to 32
    aligned_size = (class_idx + 1) * MIN_ALIGNMENT;

    if (type == 0)
      slab = g_heap.tiny[class_idx];
    else
      slab = g_heap.small[class_idx];

    while (slab)
      {
        if (slab->free_count > 0)
          return (alloc_from_slab(slab));
        slab = slab->next;
      }

    // in case of not enough space -> create new zone
    if (!(slab = init_new_slab(type, class_idx, aligned_size)))
      {
        sea_printf("Failed to allocate new zone\n");
        return (NULL);
      }

    return (alloc_from_slab(slab));
}

static void *allocate_large(size_t size)
{
  t_slab  *slab;
  size_t  total_size;

  total_size = size + sizeof(t_slab);

  total_size = (total_size + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1);

    slab = mmap(NULL, total_size, PROT_READ | PROT_WRITE,
                MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (slab == MAP_FAILED)
      {
        sea_printf("Failed to allocate large block");
        return (NULL);
      }

    slab->block_size = size;
    slab->total_blocks = 1;
    slab->free_count = 0;

    slab->next = g_heap.large;
    slab->prev = NULL;
    if (g_heap.large)
      g_heap.large->prev = slab;
    g_heap.large = slab;

    // return the pointer right after header
    return ((void *)(slab + 1));
}

__attribute__((visibility("default")))
void *sea_malloc(size_t size)
{
  void *ptr;

  if (size == 0)
    return (NULL);
  pthread_mutex_lock(&g_malloc_mutex);

  if (size <= TINY_BLOCK_MAX)
    ptr = allocate_tiny_small(size, 0); // 0 = TINY
  else if (size <= SMALL_BLOCK_MAX)
    ptr = allocate_tiny_small(size, 1); // 1 = SMALL
  else
    ptr = allocate_large(size); // LARGE

  pthread_mutex_unlock(&g_malloc_mutex);
  return (ptr);
}
